/*
 * colorgradientc.h
 *
 *  Created on: 7 juil. 2016
 *      Author: T0150270
 */

#ifndef COLORGRADIENTC_H_
#define COLORGRADIENTC_H_

#define MAX_COLORS_PER_GRADIENT 64
#define GRADIENT_RESOLUTION 255

typedef enum
{
    CG_NO_ERROR = 0,
    CG_GRADIENT_FULL
} color_gradient_error_t;

class ColorGradient
{
private:
    int number_of_colors;
    float red_list[MAX_COLORS_PER_GRADIENT];
    float green_list[MAX_COLORS_PER_GRADIENT];
    float blue_list[MAX_COLORS_PER_GRADIENT];
    float scalar_list[MAX_COLORS_PER_GRADIENT];
    unsigned int gradient[GRADIENT_RESOLUTION];
    float gradient_min;
    float gradient_max;

public:
    ColorGradient();
    virtual ~ColorGradient();

    color_gradient_error_t AddColorPoint(float red, float green, float blue, float scalar_value);
    void Generate();
    unsigned int GetUintColor(float value);
    unsigned int GetFloorUintColor(float value);

    static unsigned int ColorToUint(float red, float green, float blue);

private:
    unsigned int InterpolateColor(float value);
};

#endif /* COLORGRADIENTC_H_ */
