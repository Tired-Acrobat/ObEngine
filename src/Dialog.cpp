//Author : Sygmei
//Key : 976938ef7d46c286a2027d73f3a99467bcfa8ff0c1e10bd0016139744ef5404f4eb4d069709f9831f6de74a094944bf0f1c5bf89109e9855290336a66420376f

#include "Dialog.hpp"
#include "Animation.hpp"

namespace obe
{
	namespace Graphics
	{
		sf::RenderTexture renTex;

		//TextRenderer
		TextRenderer::TextRenderer()
		{
			currentRenderer = nullptr;
			renTex.create(1920, 1080);
		}
		void TextRenderer::createRenderer(std::string rendererType, std::string id)
		{
			Renderer* returnRenderer = nullptr;
			if (rendererType == "VisualNovel")
				returnRenderer = new Renderers::VisualNovel();
			else if (rendererType == "Shade")
				returnRenderer = new Renderers::Shade();
			else if (rendererType == "Choice")
				returnRenderer = new Renderers::Choice();
			else
				std::cout << "<Error:Dialog:TextRenderer>[createRenderer] : Failed to create Renderer of type : " << rendererType << std::endl;
			returnRenderer->load();
			returnRenderer->setFadeState(RendererState::FadeIn);
			rendererDB[id] = returnRenderer;
		}
		void TextRenderer::sendToRenderer(std::string id, std::map<std::string, std::string> tdb)
		{
			if (rendererDB.find(id) != rendererDB.end())
			{
				rendererDB[id]->addTDB(tdb);
				rendererCalls.push_back(id);
				if (currentRenderer == nullptr || !this->textRemaining())
				{
					needToRender = true;
					currentRenderer = rendererDB[id];
				}
			}
			else
				std::cout << "<Error:Dialog:TextRenderer>[sendToRenderer] : Can't find Renderer with id : " << id << std::endl;
		}
		bool TextRenderer::textRemaining() const
		{
			return rendererCalls.size() > 0;
		}
		void TextRenderer::next() const
		{
			if (rendererCalls.size() >= 1 && currentRenderer->getFadeState() != RendererState::FadeOut)
				currentRenderer->setFadeState(RendererState::FadeOut);
		}
		void TextRenderer::update(double dt)
		{
			if (currentRenderer != nullptr)
			{
				currentRenderer->update(dt);
				if (currentRenderer->getFadeState() == RendererState::End && rendererCalls.size() > 1)
				{
					rendererCalls.erase(rendererCalls.begin());
					needToRender = true;
					currentRenderer->setFadeState(RendererState::FadeIn);
					currentRenderer = rendererDB[rendererCalls[0]];
				}
				else if (currentRenderer->getFadeState() == RendererState::End && rendererCalls.size() == 1)
				{
					rendererCalls.erase(rendererCalls.begin());
					currentRenderer->setFadeState(RendererState::FadeIn);
					currentRenderer = nullptr;
				}
			}
		}
		void TextRenderer::render(sf::RenderWindow* surf)
		{
			if (needToRender && currentRenderer != nullptr)
			{
				currentRenderer->render();
				currentRenderer->fadeIn(surf);
				needToRender = false;
			}
			else if (currentRenderer != nullptr)
			{
				if (currentRenderer->getFadeState() == RendererState::FadeIn)
					currentRenderer->fadeIn(surf);
				else if (currentRenderer->getFadeState() == RendererState::Draw)
					currentRenderer->draw(surf);
				else if (currentRenderer->getFadeState() == RendererState::FadeOut)
					currentRenderer->fadeOut(surf);
			}
			else
			{
				std::cout << "<Error:Dialog:TextRenderer>[render] : No RenderClass has been specified for this TextRenderer" << std::endl;
			}
		}

		//VisualNovel
		Renderers::VisualNovel::VisualNovel() : Renderer("VisualNovel")
		{
		}
		void Renderers::VisualNovel::load()
		{
			sf::Texture* dialogLine = new sf::Texture;
			sf::Font* dialogFont = new sf::Font;
			Animation::Animation* circleAnim = new Animation::Animation;
			sf::Sprite* dialogLineSpr = new sf::Sprite;
			sf::Text* dialogText = new sf::Text;
			sf::Text* speakerText = new sf::Text;

			circleAnim->loadAnimation(System::Path("Sprites/Dialog/Loader/"), "Loader.ani.vili");
			circleAnim->playAnimation();
			circleAnim->getSprite()->setPosition(Coord::UnitVector::Screen.w - 64 - 16, Coord::UnitVector::Screen.h - 64 - 16);

			dialogLine->loadFromFile("Sprites/Dialog/textbox.png");
			dialogLineSpr->setTexture(*dialogLine);
			dialogLineSpr->setPosition(0, 760);
			dialogFont->loadFromFile("Data/Fonts/TravelingTypewriter.ttf");

			dialogText->setFont(*dialogFont);
			dialogText->setCharacterSize(32);
			dialogText->setFillColor(sf::Color(255, 255, 255, 255));
			speakerText->setFont(*dialogFont);
			speakerText->setCharacterSize(48);
			speakerText->setFillColor(sf::Color(255, 255, 255, 255));

			this->locals["circleAnim"] = circleAnim;
			this->locals["dialogLineSpr"] = dialogLineSpr;
			this->locals["dialogText"] = dialogText;
			this->locals["speakerText"] = speakerText;
			this->locals["dialogLine"] = dialogLine;
			this->locals["dialogFont"] = dialogFont;
		}
		void Renderers::VisualNovel::unload()
		{
			delete this->locals["circleAnim"].as<Animation::Animation*>();
			delete this->locals["dialogLineSpr"].as<sf::Sprite*>();
			delete this->locals["dialogText"].as<sf::Text*>();
			delete this->locals["speakerText"].as<sf::Text*>();
			delete this->locals["dialogLine"].as<sf::Texture*>();
			delete this->locals["dialogFont"].as<sf::Font*>();
			if (this->locals.find("dispTex") != locals.end())
				delete this->locals["dispTex"].as<sf::Texture*>();
			if (this->locals.find("dispSpr") != locals.end())
				delete this->locals["dispSpr"].as<sf::Sprite*>();
		}
		void Renderers::VisualNovel::render()
		{
			sf::Sprite* dialogLineSpr = this->locals["dialogLineSpr"].as<sf::Sprite*>();
			sf::Text* dialogText = this->locals["dialogText"].as<sf::Text*>();
			sf::Text* speakerText = this->locals["speakerText"].as<sf::Text*>();
			sf::Texture* dispTex = new sf::Texture;
			sf::Sprite* dispSpr = new sf::Sprite;

			renTex.clear(sf::Color(0, 0, 0, 200));

			renTex.draw(*dialogLineSpr);
			std::string speaker = vtdb[0]["speaker"];
			std::string textToSay = vtdb[0]["text"];
			speakerText->setString(sf::String(speaker));
			speakerText->setPosition((90 + (12 * (11 - speaker.size()))), 760);
			renTex.draw(*speakerText);
			int indexCounter = 0;
			int borderSize = 30;
			std::string currentPhr;
			std::vector<std::string> currentTextList;
			if (Functions::String::occurencesInString(textToSay, " ") >= 1)
				currentTextList = Functions::String::split(textToSay, " ");
			else
				currentTextList = { textToSay };
			for (unsigned int i = 0; i < currentTextList.size(); i++)
			{
				std::string testPhr = currentPhr + currentTextList[i] + " ";
				Functions::String::regenerateEncoding(testPhr);
				dialogText->setString(sf::String(testPhr));
				if (dialogText->getGlobalBounds().width > Coord::UnitVector::Screen.w - (borderSize * 2))
					currentPhr += "\n";
				currentPhr += currentTextList[i] + " ";
				Functions::String::regenerateEncoding(currentPhr);
				dialogText->setString(sf::String(currentPhr));
				indexCounter++;
			}
			Functions::String::regenerateEncoding(currentPhr);
			dialogText->setString(sf::String(currentPhr));
			dialogText->setPosition(borderSize, 840);
			renTex.draw(*dialogText);

			renTex.display();
			*dispTex = renTex.getTexture();
			dispSpr->setTexture(*dispTex);
			this->locals["dispTex"] = dispTex;
			this->locals["dispSpr"] = dispSpr;
			this->vtdb.erase(this->vtdb.begin());
		}
		void Renderers::VisualNovel::draw(sf::RenderWindow* surf)
		{
			sf::Sprite* dispSpr = this->locals["dispSpr"].as<sf::Sprite*>();
			Animation::Animation* circleAnim = this->locals["circleAnim"].as<Animation::Animation*>();

			circleAnim->playAnimation();
			surf->draw(*dispSpr);
			surf->draw(*circleAnim->getSprite());
		}

		//Shade
		Renderers::Shade::Shade() : Renderer("Shade")
		{
		}
		void Renderers::Shade::load()
		{
			sf::Font* dialogFont = new sf::Font;
			sf::Text* dialogText = new sf::Text;
			double* textAlpha = new double(255);

			dialogFont->loadFromFile("Data/Fonts/TravelingTypewriter.ttf");

			dialogText->setFont(*dialogFont);
			dialogText->setCharacterSize(32);
			dialogText->setFillColor(sf::Color(255, 255, 255, 255));

			this->locals["dialogFont"] = dialogFont;
			this->locals["dialogText"] = dialogText;
			this->locals["textAlpha"] = textAlpha;
		}
		void Renderers::Shade::unload()
		{
			delete this->locals["dialogFont"].as<sf::Font*>();
			delete this->locals["dialogText"].as<sf::Text*>();
			if (this->locals.find("dispTex") != locals.end())
				delete this->locals["dispTex"].as<sf::Texture*>();
			if (this->locals.find("dispSpr") != locals.end())
				delete this->locals["dispSpr"].as<sf::Sprite*>();
		}
		void Renderers::Shade::render()
		{
			sf::Text* dialogText = this->locals["dialogText"].as<sf::Text*>();
			sf::Texture* dispTex = new sf::Texture;
			sf::Sprite* dispSpr = new sf::Sprite;

			renTex.clear(sf::Color(0, 0, 0, 200));

			std::string textToSay = vtdb[0]["text"];
			int indexCounter = 0;
			int borderSize = 30;
			std::string currentPhr;
			std::vector<std::string> currentTextList;
			if (Functions::String::occurencesInString(textToSay, " ") >= 1)
				currentTextList = Functions::String::split(textToSay, " ");
			else
				currentTextList = { textToSay };
			for (unsigned int i = 0; i < currentTextList.size(); i++)
			{
				std::string testPhr = currentPhr + currentTextList[i] + " ";
				Functions::String::regenerateEncoding(testPhr);
				dialogText->setString(sf::String(testPhr));
				if (dialogText->getGlobalBounds().width > Coord::UnitVector::Screen.w - (borderSize * 2))
					currentPhr += "\n";
				currentPhr += currentTextList[i] + " ";
				Functions::String::regenerateEncoding(currentPhr);
				dialogText->setString(sf::String(currentPhr));
				indexCounter++;
			}
			Functions::String::regenerateEncoding(currentPhr);
			dialogText->setString(sf::String(currentPhr));
			dialogText->setPosition(borderSize, 540);
			dialogText->setFillColor(sf::Color(255, 255, 255, 255));

			renTex.display();
			*dispTex = renTex.getTexture();
			dispSpr->setTexture(*dispTex);
			this->locals["dispTex"] = dispTex;
			this->locals["dialogText"] = dialogText;
			this->locals["dispSpr"] = dispSpr;
			this->vtdb.erase(this->vtdb.begin());
		}
		void Renderers::Shade::draw(sf::RenderWindow* surf)
		{
			sf::Sprite* dispSpr = this->locals["dispSpr"].as<sf::Sprite*>();
			sf::Text* dialogText = this->locals["dialogText"].as<sf::Text*>();

			surf->draw(*dispSpr);
			surf->draw(*dialogText);
		}
		void Renderers::Shade::fadeOut(sf::RenderWindow* surf)
		{
			sf::Sprite* dispSpr = this->locals["dispSpr"].as<sf::Sprite*>();
			sf::Text* dialogText = this->locals["dialogText"].as<sf::Text*>();
			double* textAlpha = this->locals["textAlpha"].as<double*>();
			dialogText->setFillColor(sf::Color(255, 255, 255, *textAlpha));

			surf->draw(*dispSpr);
			surf->draw(*dialogText);
		}
		void Renderers::Shade::update(double dt)
		{
			if (fadeState == RendererState::FadeOut)
			{
				double* textAlpha = this->locals["textAlpha"].as<double*>();
				*textAlpha -= 5 * dt;
				if (*textAlpha < 1)
				{
					fadeState = RendererState::End;
					*textAlpha = 255;
				}
			}
		}


		//Choice
		Renderers::Choice::Choice() : Renderer("Choice")
		{
		}
		void Renderers::Choice::load()
		{
			sf::Font* dialogFont = new sf::Font;
			sf::Text* dialogText = new sf::Text;
			sf::Text* fAnswer = new sf::Text;
			sf::Text* sAnswer = new sf::Text;
			double* textAlpha = new double(255);

			dialogFont->loadFromFile("Data/Fonts/arial.ttf");

			dialogText->setFont(*dialogFont);
			dialogText->setCharacterSize(40);
			dialogText->setFillColor(sf::Color(255, 255, 255, 255));

			fAnswer->setFont(*dialogFont);
			dialogText->setCharacterSize(30);
			dialogText->setFillColor(sf::Color(255, 255, 255, 255));

			sAnswer->setFont(*dialogFont);
			sAnswer->setCharacterSize(30);
			sAnswer->setFillColor(sf::Color(255, 255, 255, 255));

			this->locals["dialogFont"] = dialogFont;
			this->locals["dialogText"] = dialogText;
			this->locals["fAnswer"] = fAnswer;
			this->locals["sAnswer"] = sAnswer;
			this->locals["textAlpha"] = textAlpha;
		}
		void Renderers::Choice::unload()
		{
			delete this->locals["dialogFont"].as<sf::Font*>();
			delete this->locals["dialogText"].as<sf::Text*>();
			delete this->locals["fAnswer"].as<sf::Text*>();
			delete this->locals["sAnswer"].as<sf::Text*>();
			if (this->locals.find("dispTex") != locals.end())
				delete this->locals["dispTex"].as<sf::Texture*>();
			if (this->locals.find("dispSpr") != locals.end())
				delete this->locals["dispSpr"].as<sf::Sprite*>();
		}
		void Renderers::Choice::render()
		{
			sf::Text* dialogText = this->locals["dialogText"].as<sf::Text*>();
			sf::Text* fAnswer = this->locals["fAnswer"].as<sf::Text*>();
			sf::Text* sAnswer = this->locals["sAnswer"].as<sf::Text*>();

			sf::Texture* dispTex = new sf::Texture;
			sf::Sprite* dispSpr = new sf::Sprite;

			renTex.clear(sf::Color(0, 0, 0, 200));

			std::string textToSay = vtdb[0]["text"];
			std::string fAnswerText = vtdb[0]["fAnswer"];
			std::string sAnswerText = vtdb[0]["sAnswer"];
			int selectedAnswer = std::stoi(vtdb[0]["selectedAnswer"]);

			Functions::String::regenerateEncoding(textToSay);
			Functions::String::regenerateEncoding(fAnswerText);
			Functions::String::regenerateEncoding(sAnswerText);

			dialogText->setString(sf::String(textToSay));
			dialogText->setPosition(960 - (dialogText->getGlobalBounds().width / 2),200);
			dialogText->setFillColor(sf::Color(255, 255, 255, 255));

			fAnswer->setString(sf::String(fAnswerText));
			fAnswer->setPosition(200, 640);
			if (selectedAnswer == 0)
				fAnswer->setFillColor(sf::Color(100, 255, 100, 255));
			else
				fAnswer->setFillColor(sf::Color(255, 255, 255, 255));

			sAnswer->setString(sf::String(sAnswerText));
			sAnswer->setPosition(1200, 640);
			if (selectedAnswer == 1)
				sAnswer->setFillColor(sf::Color(100, 255, 100, 255));
			else
				sAnswer->setFillColor(sf::Color(255, 255, 255, 255));

			renTex.draw(*dialogText);
			renTex.draw(*fAnswer);
			renTex.draw(*sAnswer);

			renTex.display();
			*dispTex = renTex.getTexture();
			dispSpr->setTexture(*dispTex);
			this->locals["dispTex"] = dispTex;
			this->locals["dialogText"] = dialogText;
			this->locals["fAnswer"] = fAnswer;
			this->locals["sAnswer"] = sAnswer;
			this->locals["dispSpr"] = dispSpr;
			this->vtdb.erase(this->vtdb.begin());
		}
		void Renderers::Choice::draw(sf::RenderWindow* surf)
		{
			sf::Sprite* dispSpr = this->locals["dispSpr"].as<sf::Sprite*>();
			sf::Text* dialogText = this->locals["dialogText"].as<sf::Text*>();

			surf->draw(*dispSpr);
			surf->draw(*dialogText);
		}


		//Renderer
		Renderer::Renderer(std::string name)
		{
			this->name = name;
		}
		void Renderer::fadeIn(sf::RenderWindow* surf)
		{
			this->draw(surf);
			fadeState = RendererState::Draw;
		}
		void Renderer::fadeOut(sf::RenderWindow* surf)
		{
			this->draw(surf);
			fadeState = RendererState::End;
		}
		void Renderer::update(double dt)
		{
		}
		void Renderer::setFadeState(int state)
		{
			fadeState = state;
		}
		int Renderer::getFadeState() const
		{
			return fadeState;
		}
		void Renderer::addTDB(std::map<std::string, std::string> tdb)
		{
			vtdb.push_back(tdb);
		}
	}
}
