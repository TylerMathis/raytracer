#include "./lib/Camera.h"
#include "./lib/Common.h"
#include "./lib/Hittable/Hittable.h"
#include "./lib/Hittable/Material/Dielectric.h"
#include "./lib/Hittable/Material/Lambertian.h"
#include "./lib/Hittable/Material/Material.h"
#include "./lib/Hittable/Material/Metal.h"
#include "./lib/Hittable/Sphere.h"
#include "./lib/Image.h"
#include "./lib/Scene.h"
#include "./lib/Vec3.h"

#include <memory>
#include <vector>

int main() {
  const double aspectRatio = 3.0 / 2.0;
  const int width = 100;
  const int height = width / aspectRatio;
  Image image("sphere.png", width, height, 3);

  Point3 origin(0, 0, -20);
  Point3 lookAt(0, 0, 0);
  Point3 up(0, 1, 0);
  const double aperture = 0;
  const double focusDist = (origin - lookAt).mag();
  const double fov = 20;
  const int samples = 50;
  const int depth = 50;
  Camera camera(origin, lookAt, up, fov, image.aspectRatio, aperture, focusDist,
                samples, depth);

  auto glass = std::make_shared<Dielectric>(1.52);
  auto solid = std::make_shared<Lambertian>(Color3(0.8, 0.5, 0.6));
  auto fore = std::make_shared<Sphere>(Point3(0, 0, 0), 2, glass);
  std::vector<std::shared_ptr<Hittable>> hittables = {fore};

  const int FRAMES = 5;
  double left = -2, right = 2, r = 0.7;
  double inc = (right - left) / FRAMES;
  for (int i = 0; i <= FRAMES; i++) {
    image.reset("image_" + std::to_string(i));

    Point3 loc(left + inc * i, 0, 0);
    auto back = std::make_shared<Sphere>(loc, r, solid);
    auto appendedHittables = hittables;
    appendedHittables.push_back(back);
    Scene scene(appendedHittables);

    scene.render(camera, image);
  }
}
