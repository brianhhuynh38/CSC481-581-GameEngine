#pragma once

#include "component.h"

#ifndef TEXTUREMESH_H
#define TEXTUREMESH_H

namespace Component {

	class TextureMeshComponent : public virtual Component {
	private:
		/** The filepath to the texture to be used */
		std::string m_textureFilePath;
		/** Texture to use for this entity */
		SDL_Texture* m_texture;
		/** whether or not this texture is visible */
		bool m_isVisible;
	public:
		
		void update() override {
		
		}

		/**
		 * returns the textureMesh's file path.
		 * @return textureFilePath
		 */
		std::string getTextureFilePath() {
			return m_textureFilePath;
		}

		/**
		 * sets the textureFilePath to the given string
		 * @param textureFilePath
		 */
		void setTextureFilePath(std::string textureFilePath) {
			m_textureFilePath = textureFilePath;
		}

		SDL_Texture* getTexture() {
			return m_texture;
		}

		void setTexture(SDL_Texture* texture) {
			m_texture = texture;
		}

		bool getIsVisible() {
			return m_isVisible;
		}

		void setIsVisable(bool visible) {
			m_isVisible = visible;
		}

	};
}

#endif