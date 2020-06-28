/*
 * colorgradientc.cpp
 *
 *  Created on: 7 juil. 2016
 *      Author: T0150270
 */

#include "ColorGradient.h"

#include <string.h>

ColorGradient::ColorGradient() : number_of_colors(0), gradient_min(0), gradient_max(0)
{
    int index;

    for (index = 0; index < MAX_COLORS_PER_GRADIENT; index++)
    {
        red_list[index] = 0.0f;
        green_list[index] = 0.0f;
        blue_list[index] = 0.0f;
        scalar_list[index] = 0.0f;
    }

    memset(gradient, 0, sizeof(gradient));
}

ColorGradient::~ColorGradient()
{
}

color_gradient_error_t ColorGradient::AddColorPoint(float red, float green, float blue, float scalar_value)
{
    if (number_of_colors >= MAX_COLORS_PER_GRADIENT)
    {
        return (CG_GRADIENT_FULL);
    }

    red_list[number_of_colors] = red;
    green_list[number_of_colors] = green;
    blue_list[number_of_colors] = blue;
    scalar_list[number_of_colors] = scalar_value;
    number_of_colors++;

    return (CG_NO_ERROR);
}

void ColorGradient::Generate()
{
    float gradient_step;

    gradient_min = scalar_list[0];
    gradient_max = scalar_list[number_of_colors - 1];
    gradient_step = (gradient_max - gradient_min) / (GRADIENT_RESOLUTION - 1);

    for (int index = 0; index < GRADIENT_RESOLUTION; index++)
    {
        gradient[index] = InterpolateColor(gradient_min + (gradient_step * index));
    }
}

unsigned int ColorGradient::GetUintColor(float value)
{
    if (value > gradient_max) value = gradient_max;
    if (value < gradient_min) value = gradient_min;
    return(gradient[(int)((GRADIENT_RESOLUTION - 1) * (value - gradient_min) / (gradient_max - gradient_min))]);
}

unsigned int ColorGradient::GetFloorUintColor(float value)
{
    int current_index;

    if (number_of_colors == 0)
        return (0);
    if (value < scalar_list[0])
    {
        return (ColorToUint(red_list[0], green_list[0], blue_list[0]));
    }
    else if (value > scalar_list[number_of_colors - 1])
    {
        return (ColorToUint(red_list[number_of_colors - 1], green_list[number_of_colors - 1], blue_list[number_of_colors - 1]));
    }

    current_index = 0;
    while(value > scalar_list[current_index + 1])
    {
        current_index++;
    }

    return(ColorToUint(red_list[current_index], green_list[current_index], blue_list[current_index]));
}

unsigned int ColorGradient::InterpolateColor(float value)
{
    int current_index;
    float color_ratio;
    float red, green, blue, alpha;

    if (number_of_colors == 0)
        return (0);
    if (value < scalar_list[0])
    {
        return (ColorToUint(red_list[0], green_list[0], blue_list[0]));
    }
    else if (value > scalar_list[number_of_colors - 1])
    {
        return (ColorToUint(red_list[number_of_colors - 1], green_list[number_of_colors - 1], blue_list[number_of_colors - 1]));
    }

    current_index = 0;
    while(value > scalar_list[current_index + 1])
    {
        current_index++;
    }

    color_ratio = (value - scalar_list[current_index]) / (scalar_list[current_index + 1] - scalar_list[current_index]);
    red = (red_list[current_index] * (1.0f - color_ratio)) + (red_list[current_index + 1] * color_ratio);
    green = (green_list[current_index] * (1.0f - color_ratio)) + (green_list[current_index + 1] * color_ratio);
    blue = (blue_list[current_index] * (1.0f - color_ratio)) + (blue_list[current_index + 1] * color_ratio);

    return(ColorToUint(red, green, blue));
}

unsigned int ColorGradient::ColorToUint(float red, float green, float blue)
{
    unsigned int red_i, green_i, blue_i;

    red_i = (red * 255.0f);
    green_i = (green * 255.0f);
    blue_i = (blue * 255.0f);

    return((blue_i << 16) | (green_i << 8) | red_i);
}
