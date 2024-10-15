#pragma once

#include "component.h"
#include "draw.h"

#include <iostream>

#ifndef TEXTUREMESH_H
#define TEXTUREMESH_H

namespace Component {

	class TextureMesh : public virtual Component {
	private:
		/** The filepath to the texture to be used */
		std::string m_textureFilePath;
		/** Texture to use for this entity */
		SDL_Texture* m_texture;
		/** whether or not this texture is visible */
		bool m_isVisible;
	public:
		
		/** 
		 * TextureMesh constructor. Constructs a textureMesh component 
		 * with the given textureFilePath as its default.
		 * @param textureFilePath to set during construction
		 */
		TextureMesh(std::string textureFilePath);

		void update() override;

		/**
		 * Returns the textureMesh's file path.
		 * @return textureFilePath
		 */
		std::string getTextureFilePath();

		/**
		 * Sets the textureFilePath to the given string.
		 * @param textureFilePath
		 */
		void setTextureFilePath(std::string textureFilePath);

		/**
		 * Gets the textureMesh's texture.
		 * @return texture
		 */
		SDL_Texture* getTexture();

		/**
		 * Sets the textureMesh's texture to the given texture.
		 * @param texture to set
		 */
		void setTexture(SDL_Texture* texture);

		/**
		 * Sets the texture by trying to load the file at the given filepath
		 * @param textureFilepath The location of the texture to load
		 */
		void setTexture(std::string textureFilepath);

		/**
		 * Gets the textureMesh's isVisible value.
		 * @return true if the textureMesh is visible
		 */
		bool getIsVisible();

		/**
		 * Sets the textureMesh's visibility to the given boolean value.
		 * @param isVisible
		 */
		void setIsVisible(bool isVisible);

	};
}

#endif