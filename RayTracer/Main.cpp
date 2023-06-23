#include "vec3.h"
#include "ray.h"
#include "color.h"

#include <fstream>

bool hit_sphere(const ray& r, const point3 centre, double radius)
{
	vec3 oc = r.origin() - centre;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius * radius;

	if ((b*b - 4*a*c) > 0)
		return true;
	return false;
}



color ray_color(const ray& r)
{
	if (hit_sphere(r, point3(0, 0, -1), 0.3))
	{
		return color(1.0, 0, 0);
	}

	auto u_dir = unit_vector(r.direction());
	auto t = 0.5 * (u_dir.y() + 1.0);
	color pixel_color = (1 - t) * color(1.0, 1.0, 1.0) + (t)*color(0.5, 0.7, 1.0);
	return pixel_color;
}


int main()
{
	std::ofstream file("image.ppm");

	auto aspect_ratio = 16.0 / 9.0;
	auto image_width = 720;
	auto image_height = image_width / aspect_ratio;

	auto view_h = 9.0;
	auto view_w = aspect_ratio * view_h;
	auto focal_length = 1.0;


	auto origin = point3(0, 0, 0);
	auto bottom_left_corner = origin - vec3(view_w / 2, view_h / 2, focal_length);

	auto ratio_w = view_w / (image_width); 
	auto ratio_h = view_h / (image_height);
	std::cout << ratio_w << " " << ratio_h;

	file << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int i = image_height - 1; i >= 0; i--)
	{
		for (int j = 0; j < image_width; j++)
		{
			auto dir = bottom_left_corner + vec3(ratio_w * (double)j , ratio_h * (double)i , 0);

			ray r(origin, dir);
			color pixel_color = ray_color(r);
			write_color(file, pixel_color);
		}
	}

	return 0;
}