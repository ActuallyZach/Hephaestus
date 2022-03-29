/*
    Camera.cpp
    Zachary lineman
    11/10/21
    
    =================
    DESCRIPTION
    =================
*/
//typedef void (*Function)();

#include "Camera.hpp"
#include "../Hephaestus.hpp"

Camera::Camera() {
//    std::cout << glm::to_string(position) << std::endl;
}

void Camera::setPosition(glm::vec3 newPosition) {
    position = newPosition;
    if (updatePositionCallback != nullptr) {
        updatePositionCallback();
    }
}

glm::vec3 Camera::getPosition() {
    return position;
}

void Camera::setRotation(glm::vec3 newRotation) {
    rotation = newRotation;
    if (updateRotationCallback != nullptr) {
        updateRotationCallback();
    }
}

glm::vec3 Camera::getRotation() {
    return rotation;
}

void Camera::setTarget(glm::vec3* newTarget) {
    target = newTarget;
    if (updateTargetCallback != nullptr) {
        updateTargetCallback();
    }
}

glm::vec3* Camera::getTarget() {
    return target;
}
void Camera::setUpdatePositionCallback(Function callback) {
    updatePositionCallback = callback;
}

void Camera::setUpdateRotationCallback(Function callback) {
    updateRotationCallback = callback;
}

void Camera::setUpdateTargetCallback(Function callback) {
    updateTargetCallback = callback;
}

glm::mat4 Camera::getView() {
    if(isUICamera) {
        return glm::mat4(1.0f);
    }
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, position);

    return view;
}