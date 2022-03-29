/*
    GameScene.cpp
    Zachary lineman
    3/19/22

    =================
    DESCRIPTION
    =================
*/

#include "GameScene.hpp"

GameScene::GameScene(Hephaestus* _engine, Function _continueFunction) {
    engine = _engine;
    continueFunction = _continueFunction;

    characterCenteringX = engine->windowWidth() / 2 - 16;
    characterCenteringY = engine->windowHeight() / 2 - 16;
}

void GameScene::setupScene() {
    scene.setShouldCheckNonPhysicsCollision(false);

    scene.setInit(std::bind(&GameScene::init, this));
    scene.setDestroy(std::bind(&GameScene::destroy, this));
    scene.setTick(std::bind(&GameScene::tick, this));
    scene.setUpdate(std::bind(&GameScene::update, this));
    scene.setRender(std::bind(&GameScene::render, this));

    scene.addKeybind(GLFW_KEY_UP, GLFW_PRESS, std::bind(&GameScene::moveCharacterUpUnit, this));
    scene.addKeybind(GLFW_KEY_DOWN, GLFW_PRESS, std::bind(&GameScene::moveCharacterDownUnit, this));
    scene.addKeybind(GLFW_KEY_LEFT, GLFW_PRESS, std::bind(&GameScene::moveCharacterLeftUnit, this));
    scene.addKeybind(GLFW_KEY_RIGHT, GLFW_PRESS, std::bind(&GameScene::moveCharacterRightUnit, this));


    scene.addKeybind(GLFW_KEY_W, GLFW_PRESS, std::bind(&GameScene::moveCameraUpUnit, this));
    scene.addKeybind(GLFW_KEY_S, GLFW_PRESS, std::bind(&GameScene::moveCameraDownUnit, this));
    scene.addKeybind(GLFW_KEY_A, GLFW_PRESS, std::bind(&GameScene::moveCameraLeftUnit, this));
    scene.addKeybind(GLFW_KEY_D, GLFW_PRESS, std::bind(&GameScene::moveCameraRightUnit, this));

    scene.addCamera(&gameCamera, true);

    /*
     * Sprite Placing
     */
    buildWorldFromTextDefinition("./CollisionTests.LDataWorld");

    character.position = { engine->windowWidth() / 2 - 16, engine->windowHeight() / 2 - 16, 0};
    character.setCollidable(true);
    scene.addSprite(&character);

    /*
     * Font Loading
     */
    scene.loadFont("./fonts/NewHiScore.ttf", 18);
    scene.addText(&playerDebugText);
    scene.addText(&cameraDebugText);
}


void GameScene::init() {

}

void GameScene::destroy() {

}

void GameScene::tick() {

}

void GameScene::update() {
    playerDebugText.text = std::string("x: ") + std::to_string(character.position.x) + std::string(", y: ") + std::to_string(character.position.y);
    cameraDebugText.text = std::string("x: ") + std::to_string(gameCamera.position.x) + std::string(", y: ") + std::to_string(gameCamera.position.y);

    gameCamera.position.y = -(character.position.y - characterCenteringY);
    gameCamera.position.x = -(character.position.x - characterCenteringX);
}

void GameScene::render() {

}

void GameScene::moveCharacterUpUnit() {
//    std::cout << "Character Position x: " << character.position.x << ", y: " << character.position.y << ". Camera Position x:" << gameCamera.position.x << ", y:" << gameCamera.position.y << std::endl;
    character.position.y += unitSizeInPixels;
//    gameCamera.position.y -= unitSizeInPixels;
}

void GameScene::moveCharacterDownUnit() {
//    std::cout << "Character Position x: " << character.position.x << ", y: " << character.position.y << ". Camera Position x:" << gameCamera.position.x << ", y:" << gameCamera.position.y << std::endl;
    character.position.y -= unitSizeInPixels;
//    gameCamera.position.y += unitSizeInPixels;
}

void GameScene::moveCharacterRightUnit() {
//    std::cout << "Character Position x: " << character.position.x << ", y: " << character.position.y << ". Camera Position x:" << gameCamera.position.x << ", y:" << gameCamera.position.y << std::endl;
    character.position.x += unitSizeInPixels;
//    gameCamera.position.x -= unitSizeInPixels;
}

void GameScene::moveCharacterLeftUnit() {
//    std::cout << "Character Position x: " << character.position.x << ", y: " << character.position.y << ". Camera Position x:" << gameCamera.position.x << ", y:" << gameCamera.position.y << std::endl;
    character.position.x -= unitSizeInPixels;
//    gameCamera.position.x += unitSizeInPixels;
}

void GameScene::moveCameraUpUnit() {
    gameCamera.position.y += unitSizeInPixels;
}

void GameScene::moveCameraDownUnit() {
    gameCamera.position.y -= unitSizeInPixels;
}

void GameScene::moveCameraRightUnit() {
    gameCamera.position.x += unitSizeInPixels;
}

void GameScene::moveCameraLeftUnit() {
    gameCamera.position.x -= unitSizeInPixels;
}

void GameScene::buildWorldFromTextDefinition(const std::string& worldPath) {
    std::ifstream file(worldPath);

    if (file.is_open()) {
        std::string line;

        bool passedDataLine = false;
        int lineCount { 0 };
        while (std::getline(file, line)) {
            if (!passedDataLine) { // Skip header line
                if(line == "=== ALL DATA ABOVE THIS LINE WILL BE IGNORED ===") {
                    passedDataLine = true;
                }
                printf("Skipped line %s\n", line.c_str());
                continue;
            }

            // Parse a regular line
            std::istringstream stringStream(line);
            vector<string> values {};

            // Break into comma seperated values
            while(stringStream.good()) {
                string subString;
                getline(stringStream, subString, ',');
                values.push_back(subString);
            }

            std::string imagePath = "./WorldSprites/" + values.at(0) + ".png"; //Append sprite name to get the image path
            glm::vec3 position = {std::stof(values.at(1)), std::stof(values.at(2)), std::stof(values.at(3))};
            glm::vec2 dimension = {std::stof(values.at(4)), std::stof(values.at(5))};
            glm::vec3 rotation = {std::stof(values.at(6)), std::stof(values.at(7)), std::stof(values.at(8))};

            Sprite constructedSprite = Sprite(imagePath, nearest, position, dimension, rotation);
            if(std::stoi(values.at(9))) {
                constructedSprite.setCollidable(true);
            } else {
                constructedSprite.setCollidable(false);
            }

            constructedSprite.immovable = true;
            worldSprites.push_back(constructedSprite);
            lineCount ++;
        }

        for(auto & worldSprite : worldSprites) {
            scene.addSprite(&worldSprite);
        }

        file.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }

}
