#include "particle.h"

Particle::Particle() 
{
	
}

Particle::Particle(unsigned int width, unsigned int height, int** data,
	std::vector<float> ranges, float resolution, int step) 
{
	std::vector <float> temp(ranges.size()/step + 1, 0);
	//~ std::vector <float> temp(ranges.size(), 0);
	_distances = temp;
	_dx = _dy = _dtheta = _weight = 0;		
	_particle_ranges = new float[ranges.size()/step + 1];	
	//~ _particle_ranges = new float[ranges.size()];	
}

void Particle::setPose (float x, float y, float theta)
{
	_x = x;
	_y = y;
	_theta = theta;
}

void Particle::randomize(unsigned int width, unsigned int height,
	int** data, float resolution)
{
	_x = std::rand() % (width) * resolution;
	_y = std::rand() % (height) * resolution;
	 
	while ((data[(int)(_x / resolution)][(int)(_y / resolution)] == - 1)
		|| (data[(int)(_x / resolution)][(int)(_y / resolution)] > 50))
	{
		_x = std::rand() % (width) * resolution;
		_y = std::rand() % (height) * resolution;
	}

	_theta = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/ (2 * PI));
}

void Particle::move()
{
	_previous_x = _x;
	_previous_y = _y;
    _x += _dx;
    _y += _dy;
    _theta += _dtheta;

    _dx = _dy = _dtheta = 0;
}

void Particle::getRanges(float angle, unsigned int width, unsigned int height,
	int** data, float resolution, float max_range, int i)
{
	int distance = 1; 
	float new_x = _x / resolution + distance * cos(angle);
	float new_y = _y / resolution + distance * sin(angle);
	
	while (((int)new_x < width && (int)new_y < height) &&
		((int)new_x >= 0 && (int)new_y >= 0) && 
		(distance * resolution <= max_range))
	{
		if ((data[(int)(new_x)][(int)(new_y)] > 50) || 
			(data[(int)(new_x)][(int)(new_y)] == -1))
		{
			_particle_ranges[i] = (distance * resolution);
			return;
		}
		else
		{
			distance ++;
			new_x = _x / resolution + distance * cos(angle);
			new_y = _y / resolution + distance * sin(angle);
		}
	}
	_particle_ranges[i] = ((distance-1) * resolution);
}

float Particle::rfidSense(std::vector<std::vector<float> > rfid_pose)
{
	float w = 0;
	float rfid_theta;
	float particle_theta = _theta;
	for (unsigned int i = 0 ; i < rfid_pose.size() ; i++)
	{
		rfid_theta = atan2(rfid_pose[i][1] - _y, rfid_pose[i][0] - _x);
		float d_th = rfid_theta - particle_theta;
		if( cos(d_th) > cos(30 * PI /180.0) ) // -30 < d_th < 30
			w = 1.0;
		else
			w = -1.0;
	}
	return w;
}

void Particle::setParticleWeight(unsigned int width, unsigned int height,
	int** data, float resolution, const std::vector<float>& ranges,
	float max_range, float increment, float angle_min,
	std::vector<std::vector<float> > rfid_pose, int step, float strictness)
{
	_weight = 0;
	float tag_w = rfidSense(rfid_pose);
	float sum = 0;
	int k = 0;	
	
	for (unsigned int i = 0 ; i < ranges.size(); i+=step)
	{
		//~ ROS_INFO_STREAM("angle = " << i*increment + angle_min );
		getRanges(i*increment + angle_min + _theta, width, height, data,
			//~ resolution, max_range, i);
			resolution, max_range, k);
		k++;
	}
	
	k = 0;
	for (unsigned int i = 0 ; i < ranges.size() ; i+=step)
	{
		if (_particle_ranges[k] / resolution <= 1)
		//~ if (_particle_ranges[i] / resolution <= 1)
		{
			_weight = 0;
			return;
		}
		else
		{	
			//~ if (_particle_ranges[i] > max_range)
				//~ _particle_ranges[i] = max_range;
			//~ _distances[i] = (fabs(ranges[i]-_particle_ranges[i]));
			//~ sum += _distances[i];
			if (_particle_ranges[k] > max_range)
				_particle_ranges[k] = max_range;
			_distances[k] = (fabs(ranges[i]-_particle_ranges[k]));
			sum += _distances[k];
			k++;
		}
	}
	
	sum = sum/(ranges.size()/step + 1);
	float sum_w = pow(1/(sum + 1), strictness);
	if(tag_w < 0.0)
	{
		_weight = sum_w * 0.0001;
	}
	else
		_weight = sum_w;
}

void Particle::calculateMotion(float previous_linear, float previous_angular, ros::Duration dt, float a1, float a2)
{
	_linear = previous_linear + noise(a1 * fabs(previous_linear) +
		a2 * fabs(previous_angular));
	_angular = previous_angular + noise(a2 * fabs(previous_linear) +
		a1 * fabs(previous_angular));
	
	if (_angular == 0)
	{
		_dx += (_linear * dt.toSec() * cosf(_theta));
		_dy += (_linear * dt.toSec() * sinf(_theta));
	}
	else
	{
		_dx += (- _linear / _angular * sinf(_theta) + _linear / _angular * 
			sinf(_theta + dt.toSec() * _angular));
	  
		_dy -= (- _linear / _angular * cosf(_theta)	+ _linear / _angular * 
			cosf(_theta + dt.toSec() * _angular));
	}
	
	_dtheta += _angular * dt.toSec() + noise(a1 * fabs(previous_linear) +
			a2 * fabs(previous_angular)) * dt.toSec();
}

float Particle::noise(float deviation) 
{
	float sum = 0;
	for ( unsigned int i = 0 ; i < 12 ; i++)
	{
		sum += - deviation + static_cast <float>
			(rand()) /( static_cast <float> (RAND_MAX/(2 * deviation)));
	}
	return 0.5*sum;
}

float Particle::getX()
{
	return _x;
}

float Particle::getY() 
{
	return _y;
}

float Particle::getTheta() 
{
	return _theta;
}

float Particle::getWeight()
{
	return _weight;
}
