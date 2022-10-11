#ifndef IMAGE_H
#define IMAGE_H

#include "Vec3.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Common.h"

#include "stdint.h"
#include "stdlib.h"

#include <iostream>
#include <string>

struct Image {
  using Color3 = _Color3<double>;

  std::string name;
  int width, height, comps, byte = 0;
  double aspectRatio;
  uint8_t *data;

  Image(const std::string name, const int width, const int height,
        const int comps)
      : name(name + ".png"), width(width), height(height), comps(comps),
        data((uint8_t *)calloc(comps * width * height, sizeof(uint8_t))),
        aspectRatio((double)width / height) {}

  void setName(const std::string newName) { name = newName + ".png"; }

  void reset(const std::string newName) {
    setName(newName);
    reset();
  }
  void reset() { byte = 0; }

  void pushPixel(const Color3 &c, const int samples) {
    Color3 sampleAndGammaCorrected = (c * (1.0 / samples)).sqrt();
    for (int i = 0; i < 3; i++)
      data[byte + i] = static_cast<int>(
          256 * common::clamp(sampleAndGammaCorrected[i], 0, 0.999));
    byte += 3;
  }

  void write() {
    std::cerr << "Writing image to " << name << "\n";
    stbi_write_png(name.c_str(), width, height, comps, data, width * comps);
  }
};

#endif
