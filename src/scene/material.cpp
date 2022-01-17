#include "scene/material.hpp"

Material::Material()
	: Ka(0.2f)
	, Kd(0.7f)
	, Ks(0.1f)
	, Ns(1.f)
	, d(1.f)
	, textureKa(0)
	, textureKd(0)
	, textureKs(0)
{
}

Material::Material(glm::vec4 Ka, glm::vec4 Kd, glm::vec4 Ks, float Ns, float d)
	: Ka(Ka)
	, Kd(Kd)
	, Ks(Ks)
	, Ns(Ns)
	, d(d)
	, textureKa(0)
	, textureKd(0)
	, textureKs(0)
{
}

Material::Material(Material&& other)
	: Ka(other.Ka)
	, Kd(other.Kd)
	, Ks(other.Ks)
	, Ns(other.Ns)
	, d(other.d)
	, textureKa(other.textureKa)
	, textureKd(other.textureKd)
	, textureKs(other.textureKs)
{
	other.textureKa = 0;
	other.textureKd = 0;
	other.textureKs = 0;
}

Material& Material::operator=(Material&& other)
{
	Ka = other.Ka;
	Kd = other.Kd;
	Ks = other.Ks;
	Ns = other.Ns;
	d = other.d;
	textureKa = other.textureKa;
	textureKd = other.textureKd;
	textureKs = other.textureKs;

	other.textureKa = 0;
	other.textureKd = 0;
	other.textureKs = 0;

	return *this;
}

Material::~Material()
{
	if (textureKa != 0)
	{
		glDeleteTextures(1, &textureKa);
	}

	if (textureKd != 0)
	{
		glDeleteTextures(1, &textureKd);
	}

	if (textureKs != 0)
	{
		glDeleteTextures(1, &textureKs);
	}
}