/*
 *
 * Copyright (c) 2018 - 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include "capaffine.h"
#include "capengine.h"
#include "caprenderer.h"


void capRenderer::camMove(float step)
{
    float lx = cosf(this->cam.yaw) * cosf(this->cam.pitch);
    float ly = sinf(this->cam.pitch);
    float lz = sinf(this->cam.yaw)*cosf(this->cam.pitch);

    this->cam.x += lx * step;
    this->cam.y += ly * step;
    this->cam.z += lz * step;
}

void capRenderer::camStrafe(float step)
{
    float lx = cosf(this->cam.yaw - CAP_PI_2);
    float lz = sinf(this->cam.yaw - CAP_PI_2);
    this->cam.x += lx * step;
    this->cam.z += lz * step;
}

void capRenderer::camYaw(float angle)
{
    this->cam.yaw += angle;
    while(this->cam.yaw < 0) this->cam.yaw += CAP_PI*2;
    while(this->cam.yaw > (CAP_PI*2)) this->cam.yaw -= CAP_PI*2;
}

void capRenderer::camPitch(float angle)
{
    const float limit = 89.0f * CAP_PI / 180.0f;

    this->cam.pitch += angle;

    if(this->cam.pitch < -limit)
        this->cam.pitch = -limit;

    if(this->cam.pitch > limit)
        this->cam.pitch = limit;
}

void capRenderer::camLift(float step)
{
    this->cam.y += step;
}


void capRenderer::enableLight()
{
    this->light.lightFlags |= CLF_ENABLED;
}

void capRenderer::disableLight()
{
    this->light.lightFlags &= ~CLF_ENABLED;
}

capRenderer::capRenderer() {
    int i;
    for(i = 0; i<=CAV_SINGLE_MAX; i++)
    {
        this->views[i].orthoZoom = 100.0f;
        this->views[i].orthoClip = 300.0f;
        this->views[i].rasterSize = 0.1f;
    }

    this->views[CAV_NVMCAM].orthoZoom = 1.0f;
    this->views[CAV_CAVER].fov = 60.0f;
    this->views[CAV_XSECT].orthoClip = 0.150f;

    this->currentView = CAV_CAVER;
	this->sectColor.r = 255;
	this->sectColor.g = 255;
	this->sectColor.b = 255;
	this->sectColor.a = 64;
}

capGenericPoint capRenderer::screenPosToNvm(float x, float y) {
    capGenericPoint p;
    p.x = x; p.y = y; p.z = 0;
    return screenPosToNvm(p);
}

bool capRenderer::screenshot(const char *saveas) {
    return false;
}
