#pragma once
#include "RenderInfo.h"
#include "AntiAliasingInfo.h"
#include "Text.h"
class Shader;
class Font;

class Hud
{
public:
	Hud(Font& font, Shader& shader);
	~Hud();

	void Update(int fps, const RenderInfo& renderInfo, const AntiAliasingInfo& aaInfo) ;
	void Render() const;

protected:
	Font& m_font;
	Shader& m_shader;
	Text m_fpsText;
	Text m_aaText;
};

