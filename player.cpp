#include <sstream>

#include "player.h"
#include "draw.h"
#include "vector2D.h"
#include "collisions.h"

namespace Entities {

	/**
	 * Constructs the Player and initializes all fields
	 */
	Player::Player(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
		std::string textureFilepath, bool isStationary, bool affectedByPhysics, float jumpVectorX, float jumpVectorY, float maxSpeed) {

		m_scale = new Utils::Vector2D(scaleX, scaleY);
		m_position = new Utils::Vector2D(positionX, positionY);
		m_size = new Utils::Vector2D(width, height);
		m_mass = mass;

		m_velocity = new Utils::Vector2D(0.0, 0.0);
		m_acceleration = new Utils::Vector2D(0.0, 0.0);
		m_jumpVector = new Utils::Vector2D(jumpVectorX, jumpVectorY);

		m_textureFilepath = textureFilepath;
		//m_texture = Render::loadTexture(textureFilepath);

		m_isStationary = isStationary;
		m_affectedByPhysics = affectedByPhysics;
		m_maxSpeed = maxSpeed;
		isGrounded = false;
		m_colliders = new std::list<SDL_Rect>();
		m_colliders->emplace_back(SDL_Rect() = {(int) positionX, (int) positionY, (int) (scaleX * width), (int) (scaleY * height)});
	}

	/**
	 * Returns the player's max speed
	 * @returns The maximum amount of speed the object can go
	 */
	float Player::getMaxSpeed(void) {
        return m_maxSpeed;
	}

	/**
	 * Change the maximum speed the player can go
	 */
	void Player::setMaxSpeed(float maxSpeed) {
        m_maxSpeed = maxSpeed;
	}

	/**
	 * Returns jumpVector of player
	 * @return jumpVector of player
	 */
	Utils::Vector2D* Player::getJumpVector() {
		return m_jumpVector;
	}

	/**
	 * Sets players jumpVector value to the given
	 * @param jumpVector value
	 */
	void Player::setJumpVector(Utils::Vector2D jVector) {
		*m_jumpVector = jVector;
	}

	/**
	 * Returns true if on ground
	 * @return true if on ground
	 */
	bool Player::getIsGrounded() {
		return isGrounded;
	}

	/**
	 * sets players isGrounded value to the given
	 * @param grounded value
	 */
	void Player::setIsGrounded(bool grounded) {
		isGrounded = grounded;
	}

	std::string Player::toString() {
		// Create string to serialize all fields within the Entity
		// Each field is delineated by a newline
		std::stringstream ss;
		// Stringify ID
		ss << m_uuid << "\n";
		// Stringify positional and physics vectors
		ss << getPosition()->toString();
		ss << getVelocity()->toString();
		ss << getAcceleration()->toString();
		// Stringify scale, size, and mass
		ss << getScale()->toString();
		ss << getSize()->toString();
		ss << getMass() << "\n";
		// Stringify max velocity and acceleration values
		ss << m_velocity_max << "\n";
		ss << m_acceleration_max << "\n";
		// Get filepath to SDLTexture
		ss << m_textureFilepath << "\n";

		//std::cout << "Filepath inside Player toString: " << m_textureFilepath << "\n";
		

		// Stringifies each SDL_Rect Collider
		for (SDL_Rect collider : *m_colliders) {
			ss << collider.x << "," << collider.y << "," << collider.w << "," << collider.h << "\t";
		}
		ss << "\n";
		// Stringifies each of the bools: stationary, affectedByPhysics, visible
		ss << m_isStationary << "\n" << m_affectedByPhysics << "\n" << m_isVisible << "\n";
		//Stringify Player-specific stuff
		ss << getMaxSpeed() << "\n" << getIsGrounded() << "\n" << getJumpVector()->toString();

		std::string ssString = ss.str();

		return ssString;
	}

	Player* Player::fromString(const std::string& data) {
		std::stringstream ss(data);
		std::string line;

		// Helper functions
		auto getFloat = [&]() { std::getline(ss, line); return std::stof(line); };
		auto getInt = [&]() { std::getline(ss, line); return std::stoi(line); };
		auto getBool = [&]() { return getInt() != 0; };

		std::getline(ss, line);
		int uuid = getInt(); // Use to identify the object

		Utils::Vector2D position = Utils::Vector2D::fromString(ss);
		Utils::Vector2D velocity = Utils::Vector2D::fromString(ss);
		Utils::Vector2D acceleration = Utils::Vector2D::fromString(ss);
		Utils::Vector2D scale = Utils::Vector2D::fromString(ss);
		Utils::Vector2D size = Utils::Vector2D::fromString(ss);

		float mass = getFloat();
		float velocity_max = getFloat();
		float acceleration_max = getFloat();

		std::getline(ss, line);

		std::string textureFilePath = line;

		std::list<SDL_Rect>* colliders = new std::list<SDL_Rect>();
		std::getline(ss, line);
		std::stringstream colliderStream(line);
		while (std::getline(colliderStream, line, '\t')) {
			SDL_Rect rect;
			sscanf_s(line.c_str(), "%d,%d,%d,%d", &rect.x, &rect.y, &rect.w, &rect.h);
			colliders->push_back(rect);
		}

		bool isStationary = getBool();
		bool affectedByPhysics = getBool();
		bool isVisible = getBool();
		float maxSpeed = getFloat();
		bool isGrounded = getBool();

		Utils::Vector2D jumpVector = Utils::Vector2D::fromString(ss);

		Player* entity = new Player(scale.x, scale.y, position.x, position.y, size.x, size.y, mass,
			textureFilePath, isStationary, affectedByPhysics, jumpVector.x, jumpVector.y, maxSpeed);

		// Set other fields if needed
		entity->setIsGrounded(isGrounded);
		entity->setUUID(uuid);
		entity->setTexture(textureFilePath);

		return entity;
	}
}