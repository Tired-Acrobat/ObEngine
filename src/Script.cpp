//Author : Sygmei
//Key : 976938ef7d46c286a2027d73f3a99467bcfa8ff0c1e10bd0016139744ef5404f4eb4d069709f9831f6de74a094944bf0f1c5bf89109e9855290336a66420376f

#include "Script.hpp"

namespace obe
{
	Types::any::erasure::~erasure() {}
	namespace Script
	{
		CoreHook hookCore;

		void CoreHook::getValue(kaguya::State* lua, std::string name)
		{
			if (containerMap.find(name) != containerMap.end())
			{
				std::string hookType = containerMap[name].first;
				if (hookType == Functions::Type::getClassType<Console::Console*>())
					(*lua)["Hook"][name] = containerMap[name].second->as<Console::Console*>();
				else if (hookType == Functions::Type::getClassType<Cursor::Cursor*>())
					(*lua)["Hook"][name] = containerMap[name].second->as<Cursor::Cursor*>();
				else if (hookType == Functions::Type::getClassType<Input::KeyBinder*>())
					(*lua)["Hook"][name] = containerMap[name].second->as<Input::KeyBinder*>();
				else if (hookType == Functions::Type::getClassType<Math::MathExp*>())
					(*lua)["Hook"][name] = containerMap[name].second->as<Math::MathExp*>();
				else if (hookType == Functions::Type::getClassType<Graphics::TextRenderer*>())
					(*lua)["Hook"][name] = containerMap[name].second->as<Graphics::TextRenderer*>();
				else if (hookType == Functions::Type::getClassType<Script::TriggerDatabase*>())
					(*lua)["Hook"][name] = containerMap[name].second->as<TriggerDatabase*>();
				else if (hookType == Functions::Type::getClassType<Script::TriggerGroup*>())
					(*lua)["Hook"][name] = containerMap[name].second->as<TriggerGroup*>();
				else
					throw aube::ErrorHandler::Raise("ObEngine.Script.CoreHook.UnknownHookType", { {"type", hookType}, {"name", name} });
			}
			else
				throw aube::ErrorHandler::Raise("ObEngine.Script.CoreHook.UnknownHookName", { { "name", name } });
		}

		Types::any* CoreHook::getPointer(std::string name)
		{
			if (containerMap.find(name) != containerMap.end())
			{
				return containerMap[name].second;
			}
			throw aube::ErrorHandler::Raise("ObEngine.Script.CoreHook.UnknownPointerName", { { "name", name } });
		}

		void loadLib(kaguya::State* lua, std::string lib)
		{
			if (Functions::String::occurencesInString(lib, ".") >= 1)
			{
				if (Functions::String::split(lib, ".")[0] == "Core")
					loadCoreLib(lua, Functions::Vector::getSubVector(Functions::String::split(lib, "."), 1, 0));
				else
					throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.UnknownNamespace", { {"name", lib} });
			}
			else
				throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.NoNamespaceProvided");
		}

		void loadHook(kaguya::State* lua, std::string hookname)
		{
			hookCore.getValue(lua, hookname);
		}

		void loadCoreLib(kaguya::State* lua, std::vector<std::string> lib)
		{
			std::vector<std::string> importedLibs = (*lua)["Core"]["ImportedLibs"];
			bool alreadyImported = false;
			for (int i = 0; i < importedLibs.size(); i++)
			{
				std::vector<std::string> cLibComp = Functions::String::split(importedLibs[i], ".");
				if (lib == cLibComp)
				{
					alreadyImported = true;
					break;
				}
				else if (cLibComp.size() < lib.size())
				{
					bool sVecLibAF = true;
					if (cLibComp.size() > 0)
					{
						for (int j = 0; j < cLibComp.size(); j++)
							if (cLibComp[j] != lib[j]) { sVecLibAF = false; }
					}
					else
						sVecLibAF = false;
					if (sVecLibAF) { alreadyImported = true; break; }
				}
			}
			if (!alreadyImported)
			{
				bool all = lib[0] == "*";
				if (all) { lib.clear(); lib.push_back("*"); }
				bool found = false;
				if (lib[0] == "Animation" || all) { CoreLib::loadAnimation(lua, (all) ? std::vector<std::string>{"Animation"} : lib);    found = true; }
				if (lib[0] == "Canvas" || all) { CoreLib::loadCanvas(lua, (all) ? std::vector<std::string>{"Canvas"} : lib);    found = true; }
				if (lib[0] == "Collision" || all) { CoreLib::loadCollision(lua, (all) ? std::vector<std::string>{"Collision"} : lib);    found = true; }
				if (lib[0] == "Console" || all) { CoreLib::loadConsole(lua, (all) ? std::vector<std::string>{"Console"} : lib);    found = true; }
				if (lib[0] == "Constants" || all) { CoreLib::loadConstants(lua, (all) ? std::vector<std::string>{"Constants"} : lib);    found = true; }
				if (lib[0] == "Coordinates" || all) { CoreLib::loadCoordinates(lua, (all) ? std::vector<std::string>{"Coordinates"} : lib); found = true; }
				if (lib[0] == "Cursor" || all) { CoreLib::loadCursor(lua, (all) ? std::vector<std::string>{"Cursor"} : lib);    found = true; }
				if (lib[0] == "Dialog" || all) { CoreLib::loadDialog(lua, (all) ? std::vector<std::string>{"Dialog"} : lib);    found = true; }
				if (lib[0] == "KeyBind" || all) { CoreLib::loadKeyBind(lua, (all) ? std::vector<std::string>{"KeyBind"} : lib);    found = true; }
				if (lib[0] == "LevelSprite" || all) { CoreLib::loadLevelSprite(lua, (all) ? std::vector<std::string>{"LevelSprite"} : lib);    found = true; }
				if (lib[0] == "Light" || all) { CoreLib::loadLight(lua, (all) ? std::vector<std::string>{"Light"} : lib);    found = true; }
				if (lib[0] == "MathExp" || all) { CoreLib::loadMathExp(lua, (all) ? std::vector<std::string>{"MathExp"} : lib);    found = true; }
				if (lib[0] == "Package" || all) { CoreLib::loadPackage(lua, (all) ? std::vector<std::string>{"Package"} : lib);    found = true; }
				if (lib[0] == "Particle" || all) { CoreLib::loadParticle(lua, (all) ? std::vector<std::string>{"Particle"} : lib);    found = true; }
				if (lib[0] == "Path" || all) { CoreLib::loadPath(lua, (all) ? std::vector<std::string>{"Path"} : lib);    found = true; }
				if (lib[0] == "SFML" || all) { CoreLib::loadSFML(lua, (all) ? std::vector<std::string>{"SFML"} : lib);    found = true; }
				if (lib[0] == "Sound" || all) { CoreLib::loadSound(lua, (all) ? std::vector<std::string>{"Sound"} : lib); found = true; }
				if (lib[0] == "STD" || all) { CoreLib::loadSTD(lua, (all) ? std::vector<std::string>{"STD"} : lib);    found = true; }
				if (lib[0] == "Trigger" || all) { CoreLib::loadTrigger(lua, (all) ? std::vector<std::string>{"Trigger"} : lib);    found = true; }
				if (lib[0] == "Utils" || all) { CoreLib::loadUtils(lua, (all) ? std::vector<std::string>{"Utils"} : lib);    found = true; }
				if (lib[0] == "Vili" || all) { CoreLib::loadVili(lua, (all) ? std::vector<std::string>{"DataParser"} : lib); found = true; }
				if (!found)
				{
					std::string libName = Functions::Vector::join(lib, ".");
					throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.UnknownCoreLib", { {"lib", libName} });
				}
			}
			else
			{
				std::cout << "<Warning:Script:*>[loadCoreLib] : Core.";
				for (int i = 0; i < lib.size(); i++)
				{
					if (i != lib.size() - 1) std::cout << lib[i] << ".";
					else std::cout << lib[i];
				}
				std::cout << " is already loaded" << std::endl;
			}
		}

		void registerLib(kaguya::State* lua, std::string lib)
		{
			std::string frag = std::string("table.insert(Core.ImportedLibs, ") + std::string("\"") + std::string(lib) + std::string("\")");
			(*lua)(frag);
		}

		void loadBaseLib(kaguya::State* lua)
		{
			(*lua)["CPP_Import"] = &loadLib;
			(*lua)["CPP_Hook"] = &loadHook;
		}

		void CoreLib::loadAnimation(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Animation"])) (*lua)["Core"]["Animation"] = kaguya::NewTable();
			if (importAll || args[1] == "AnimationGroup")
			{
				(*lua)["Core"]["Animation"]["AnimationGroup"].setClass(kaguya::UserdataMetatable<Animation::AnimationGroup>()
					.addFunction("setGroupClock", &Animation::AnimationGroup::setGroupClock)
					.addFunction("setGroupLoop", &Animation::AnimationGroup::setGroupLoop)
					.addFunction("getGroupName", &Animation::AnimationGroup::getGroupName)
					.addFunction("getGroupClock", &Animation::AnimationGroup::getGroupClock)
					.addFunction("getGroupIndex", &Animation::AnimationGroup::getGroupIndex)
					.addFunction("isGroupOver", &Animation::AnimationGroup::isGroupOver)
					.addFunction("pushTexture", &Animation::AnimationGroup::pushTexture)
					.addFunction("removeTextureByIndex", &Animation::AnimationGroup::removeTextureByIndex)
					.addFunction("reset", &Animation::AnimationGroup::reset)
					.addFunction("next", &Animation::AnimationGroup::next)
					.addFunction("previous", &Animation::AnimationGroup::previous)
					.addFunction("forceNext", &Animation::AnimationGroup::forceNext)
					.addFunction("forcePrevious", &Animation::AnimationGroup::forcePrevious)
				);
				foundPart = true;
			}
			if (importAll || args[1] == "Animation")
			{
				(*lua)["Core"]["Animation"]["Animation"].setClass(kaguya::UserdataMetatable<Animation::Animation>()
					.addFunction("getAnimationName", &Animation::Animation::getAnimationName)
					.addFunction("getAnimationClock", &Animation::Animation::getAnimationClock)
					.addFunction("getAnimationGroup", &Animation::Animation::getAnimationGroup)
					.addFunction("getCurrentAnimationGroup", &Animation::Animation::getCurrentAnimationGroup)
					.addFunction("getAllAnimationGroupName", &Animation::Animation::getAllAnimationGroupName)
					.addFunction("getAnimationPlayMode", &Animation::Animation::getAnimationPlayMode)
					.addFunction("getAnimationStatus", &Animation::Animation::getAnimationStatus)
					.addFunction("isAnimationOver", &Animation::Animation::isAnimationOver)
					.addFunction("getSpriteOffsetX", &Animation::Animation::getSpriteOffsetX)
					.addFunction("getSpriteOffsetY", &Animation::Animation::getSpriteOffsetY)
					.addFunction("getPriority", &Animation::Animation::getPriority)
				);
				foundPart = true;
			}
			if (importAll || args[1] == "Animator")
			{
				(*lua)["Core"]["Animation"]["Animator"].setClass(kaguya::UserdataMetatable<Animation::Animator>()
					.addFunction("getAnimation", &Animation::Animator::getAnimation)
					.addFunction("getAllAnimationName", &Animation::Animator::getAllAnimationName)
					.addFunction("loadAnimator", &Animation::Animator::loadAnimator)
					.addFunction("getKey", &Animation::Animator::getKey)
					.addFunction("setKey", &Animation::Animator::setKey)
					.addFunction("textureChanged", &Animation::Animator::textureChanged)
					.addFunction("getSpriteOffsetX", &Animation::Animator::getSpriteOffsetX)
					.addFunction("getSpriteOffsetY", &Animation::Animator::getSpriteOffsetY)
					.addOverloadedFunctions("setPath",
						static_cast<void (Animation::Animator::*)(std::string)>(&Animation::Animator::setPath),
						static_cast<void (Animation::Animator::*)(System::Path)>(&Animation::Animator::setPath)
					)
				);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.AnimationImportError", { {"lib", Functions::Vector::join(args, ".") } });
		}

		void CoreLib::loadCanvas(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Canvas"])) (*lua)["Core"]["Canvas"] = kaguya::NewTable();
			if (importAll || args[1] == "Cursor")
			{
				(*lua)["Core"]["Canvas"]["RequirePair"].setClass(kaguya::UserdataMetatable<std::pair<std::string, std::string>>()
					.addFunction("first", &std::pair<std::string, std::string>::first)
					.addFunction("second", &std::pair<std::string, std::string>::second)
				);
				(*lua)["Core"]["Canvas"]["Element"].setClass(kaguya::UserdataMetatable<Graphics::Element>());
				(*lua)["Core"]["Canvas"]["Configurable"].setClass(kaguya::UserdataMetatable<Graphics::Configurable, Graphics::Element>()
					.addFunction("init", &Graphics::Configurable::init)
				);
				(*lua)["Core"]["Canvas"]["Drawable"].setClass(
					kaguya::UserdataMetatable<Graphics::Drawable, Graphics::Configurable>()
				);
				(*lua)["Core"]["Canvas"]["Colorable"].setClass(
					kaguya::UserdataMetatable<Graphics::Colorable, Graphics::Drawable>()
				);
				(*lua)["Core"]["Canvas"]["Transformable"].setClass(
					kaguya::UserdataMetatable<Graphics::Transformable, Graphics::Configurable>()
				);
				(*lua)["Core"]["Canvas"]["CanvasElement"].setClass(
					kaguya::UserdataMetatable<Graphics::CanvasElement, Graphics::Drawable>()
				);
				(*lua)["Core"]["Canvas"]["Line"].setClass(
					kaguya::UserdataMetatable<Graphics::Line, kaguya::MultipleBase<Graphics::CanvasElement, Graphics::Colorable>>()
				);
				(*lua)["Core"]["Canvas"]["Rectangle"].setClass(
					kaguya::UserdataMetatable<Graphics::Rectangle, kaguya::MultipleBase<Graphics::CanvasElement, Graphics::Colorable, Graphics::Transformable>>()
				);
				(*lua)["Core"]["Canvas"]["Text"].setClass(
					kaguya::UserdataMetatable<Graphics::Text, kaguya::MultipleBase<Graphics::CanvasElement, Graphics::Colorable, Graphics::Transformable>>()
				);
				(*lua)["Core"]["Canvas"]["Circle"].setClass(
					kaguya::UserdataMetatable<Graphics::Circle, kaguya::MultipleBase<Graphics::CanvasElement, Graphics::Colorable, Graphics::Transformable>>()
				);
				(*lua)["Core"]["Canvas"]["Canvas"].setClass(
					kaguya::UserdataMetatable<Graphics::Canvas>()
					.setConstructors<Graphics::Canvas(kaguya::State*, unsigned int, unsigned int)>()
					.addFunction("Line", &Graphics::Canvas::line)
					.addFunction("Rectangle", &Graphics::Canvas::rectangle)
					.addFunction("Text", &Graphics::Canvas::text)
					.addFunction("Circle", &Graphics::Canvas::circle)
					.addFunction("Get", &Graphics::Canvas::get)
					.addFunction("render", &Graphics::Canvas::render)
					.addFunction("setTarget", &Graphics::Canvas::setTarget)
				);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.CanvasImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}

		void CoreLib::loadCollision(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Collision"])) (*lua)["Core"]["Collision"] = kaguya::NewTable();
			if (importAll || args[1] == "PolygonalCollider")
			{
				(*lua)["Core"]["Collision"]["PolygonalCollider"].setClass(kaguya::UserdataMetatable<Collision::PolygonalCollider>()
					.setConstructors<Collision::PolygonalCollider(std::string)>()
					.addFunction("addExcludedTag", &Collision::PolygonalCollider::addExcludedTag)
					.addFunction("addPoint", &Collision::PolygonalCollider::addPoint)
					.addFunction("addTag", &Collision::PolygonalCollider::addTag)
					.addFunction("clearHighlights", &Collision::PolygonalCollider::clearHighlights)
					.addFunction("deletePoint", &Collision::PolygonalCollider::deletePoint)
					.addFunction("doesCollide", &Collision::PolygonalCollider::doesCollide)
					.addFunction("doesCollideWithTag", &Collision::PolygonalCollider::doesCollideWithTags)
					.addFunction("doesHaveTag", &Collision::PolygonalCollider::doesHaveTag)
					.addFunction("doesPathCollide", &Collision::PolygonalCollider::doesPathCollide)
					.addFunction("findClosestPoint", &Collision::PolygonalCollider::findClosestPoint)
					.addFunction("getCollidedCollidersWithTags", &Collision::PolygonalCollider::getCollidedCollidersWithTags)
					.addFunction("getDistanceFromPoint", &Collision::PolygonalCollider::getDistanceFromPoint)
					.addFunction("getID", &Collision::PolygonalCollider::getID)
					.addFunction("getMasterPointPosition", &Collision::PolygonalCollider::u_getMasterPointPosition)
					.addFunction("getOrigin", &Collision::PolygonalCollider::getOrigin)
					.addFunction("getParentID", &Collision::PolygonalCollider::getParentID)
					.addFunction("getPath", &Collision::PolygonalCollider::getPath)
					.addFunction("getPointPosition", &Collision::PolygonalCollider::u_getPointPosition)
					.addFunction("getPointRelativePosition", &Collision::PolygonalCollider::u_getPointRelativePosition)
					.addFunction("getPointsAmount", &Collision::PolygonalCollider::getPointsAmount)
					.addFunction("getPosition", &Collision::PolygonalCollider::u_getPosition)
					.addFunction("getSelected", &Collision::PolygonalCollider::getSelected)
					.addFunction("getSideAngle", &Collision::PolygonalCollider::getSideAngle)
					.addFunction("getSideContainingPoint", &Collision::PolygonalCollider::getSideContainingPoint)
					.addFunction("getSideLength", &Collision::PolygonalCollider::getSideLength)
					.addFunction("hasMasterPoint", &Collision::PolygonalCollider::hasMasterPoint)
					.addFunction("hasPoint", &Collision::PolygonalCollider::hasPoint)
					.addFunction("highlightLine", &Collision::PolygonalCollider::highlightLine)
					.addFunction("highlightPoint", &Collision::PolygonalCollider::highlightPoint)
					.addFunction("isPointInBoundingBox", &Collision::PolygonalCollider::isPointInBoundingBox)
					.addFunction("move", &Collision::PolygonalCollider::u_move)
					.addFunction("movePoint", &Collision::PolygonalCollider::u_movePoint)
					.addFunction("removeOrigin", &Collision::PolygonalCollider::removeOrigin)
					.addFunction("removeTag", &Collision::PolygonalCollider::removeTag)
					.addFunction("setDrawOffset", &Collision::PolygonalCollider::setDrawOffset)
					.addFunction("setOrigin", &Collision::PolygonalCollider::setOrigin)
					.addFunction("setPointPosition", &Collision::PolygonalCollider::u_setPointPosition)
					.addFunction("setPointRelativePosition", &Collision::PolygonalCollider::u_setPointRelativePosition)
					.addFunction("setPointPositionFromMaster", &Collision::PolygonalCollider::u_setPointPositionFromMaster)
					.addFunction("setPosition", &Collision::PolygonalCollider::u_setPosition)
					.addFunction("setPositionFromMaster", &Collision::PolygonalCollider::u_setPositionFromMaster)
					.addFunction("setSelected", &Collision::PolygonalCollider::setSelected)
					.addFunction("testAllColliders", &Collision::PolygonalCollider::testAllColliders)
				);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.CollisionImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadConsole(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Console"])) (*lua)["Core"]["Console"] = kaguya::NewTable();
			if (importAll || args[1] == "Console")
			{
				(*lua)["Core"]["Console"]["Console"].setClass(kaguya::UserdataMetatable<Console::Console>()
					.addFunction("getCommand", &Console::Console::getCommand)
					.addFunction("hasCommand", &Console::Console::hasCommand)
					.addFunction("scroll", &Console::Console::scroll)
					.addFunction("getInputBufferContent", &Console::Console::getInputBufferContent)
					.addFunction("setInputBufferContent", &Console::Console::setInputBufferContent)
					.addFunction("insertInputBufferContent", &Console::Console::insertInputBufferContent)
					.addFunction("createStream", &Console::Console::createStream)
					.addFunction("isConsoleVisible", &Console::Console::isConsoleVisible)
					.addFunction("setConsoleVisibility", &Console::Console::setConsoleVisibility)
				);
				foundPart = true;
			}
			if (importAll || args[1] == "Stream")
			{
				(*lua)["Core"]["Console"]["Stream"].setClass(kaguya::UserdataMetatable<Console::Console::Stream>()
					.addOverloadedFunctions("write", 
						static_cast<Console::Console::Message* (Console::Console::Stream::*)(std::string)>
						(&Console::Console::Stream::streamPush),
						static_cast<Console::Console::Message* (Console::Console::Stream::*)(std::string, int, int, int, int)>
						(&Console::Console::Stream::streamPush)
					)
					.addFunction("setColor", &Console::Console::Stream::setColor)
					.addFunction("getR", &Console::Console::Stream::getR)
					.addFunction("getG", &Console::Console::Stream::getG)
					.addFunction("getB", &Console::Console::Stream::getB)
					.addFunction("getA", &Console::Console::Stream::getA)
				);
				foundPart = true;
			}
			if (importAll || args[1] == "Message")
			{
				(*lua)["Core"]["Console"]["Message"].setClass(kaguya::UserdataMetatable<Console::Console::Message>()
					.addFunction("getFormatedMessage", &Console::Console::Message::getFormatedMessage)
					.addFunction("getHeader", &Console::Console::Message::getHeader)
					.addFunction("getMessage", &Console::Console::Message::getMessage)
					.addFunction("getR", &Console::Console::Message::getR)
					.addFunction("getG", &Console::Console::Message::getG)
					.addFunction("getB", &Console::Console::Message::getB)
					.addFunction("getA", &Console::Console::Message::getA)
					.addFunction("getType", &Console::Console::Message::getType)
					.addFunction("setMessage", &Console::Console::Message::setMessage)
					.addFunction("setColor", &Console::Console::Message::setColor)
					);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.ConsoleImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadConstants(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Constants"])) (*lua)["Core"]["Constants"] = kaguya::NewTable();
			if (importAll)
			{
				(*lua)["Core"]["Constants"]["Screen"] = kaguya::NewTable();
				(*lua)["Core"]["Constants"]["Screen"]["Width"] = Coord::UnitVector::Screen.w;
				(*lua)["Core"]["Constants"]["Screen"]["Height"] = Coord::UnitVector::Screen.h;
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.ConstantsImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}

		void CoreLib::loadCoordinates(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Coordinates"])) (*lua)["Core"]["Coordinates"] = kaguya::NewTable();
			if (importAll)
			{
				(*lua)["Core"]["Coordinates"]["ViewPercentage"] = Coord::Units::ViewPercentage;
				(*lua)["Core"]["Coordinates"]["ViewPixels"] = Coord::Units::ViewPixels;
				(*lua)["Core"]["Coordinates"]["ViewUnit"] = Coord::Units::ViewUnits;
				(*lua)["Core"]["Coordinates"]["WorldPixels"] = Coord::Units::WorldPixels;
				(*lua)["Core"]["Coordinates"]["WorldUnits"] = Coord::Units::WorldUnits;
				(*lua)["Core"]["Coordinates"]["UnitVector"].setClass(kaguya::UserdataMetatable<Coord::UnitVector>()
					.addProperty("x", &Coord::UnitVector::x)
					.addProperty("y", &Coord::UnitVector::y)
					.addProperty("unit", &Coord::UnitVector::unit)
					.addFunction("to", static_cast<Coord::UnitVector (Coord::UnitVector::*)(Coord::Units) const>(&Coord::UnitVector::to))
					.addFunction("add", &Coord::UnitVector::add)
					.addOverloadedFunctions("set", 
						static_cast<void (Coord::UnitVector::*)(const double&, const double&)>(&Coord::UnitVector::set),
						static_cast<void (Coord::UnitVector::*)(const Coord::UnitVector&)>(&Coord::UnitVector::set)
					)
				);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.CoordinatesImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}

		void CoreLib::loadCursor(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Cursor"])) (*lua)["Core"]["Cursor"] = kaguya::NewTable();
			if (importAll || args[1] == "Cursor")
			{
				(*lua)["Core"]["Cursor"]["Cursor"].setClass(kaguya::UserdataMetatable<Cursor::Cursor>()
					.addFunction("getClicked", &Cursor::Cursor::getClicked)
					.addFunction("getPressed", &Cursor::Cursor::getPressed)
					.addFunction("getReleased", &Cursor::Cursor::getReleased)
					.addFunction("getX", &Cursor::Cursor::getX)
					.addFunction("getY", &Cursor::Cursor::getY)
					.addFunction("selectCursor", &Cursor::Cursor::selectCursor)
					.addFunction("selectKey", &Cursor::Cursor::selectKey)
					.addFunction("setPosition", &Cursor::Cursor::setPosition)
					.addFunction("setX", &Cursor::Cursor::setX)
					.addFunction("setY", &Cursor::Cursor::setY)
					);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.CursorImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadDialog(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Dialog"])) (*lua)["Core"]["Dialog"] = kaguya::NewTable();
			if (importAll || args[1] == "TextRenderer")
			{
				(*lua)["Core"]["Dialog"]["TextRenderer"].setClass(kaguya::UserdataMetatable<Graphics::TextRenderer>()
					.addFunction("createRenderer", &Graphics::TextRenderer::createRenderer)
					.addFunction("sendToRenderer", &Graphics::TextRenderer::sendToRenderer)
					.addFunction("textRemaining", &Graphics::TextRenderer::textRemaining)
					.addFunction("next", &Graphics::TextRenderer::next)
					);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.DialogImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadKeyBind(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["KeyBind"])) (*lua)["Core"]["KeyBind"] = kaguya::NewTable();
			if (importAll || args[1] == "KeyBinder")
			{
				(*lua)["Core"]["KeyBind"]["KeyBinder"].setClass(kaguya::UserdataMetatable<Input::KeyBinder>()
					.addFunction("connectAction", &Input::KeyBinder::connectAction)
					.addFunction("isActionDisabled", &Input::KeyBinder::isActionDisabled)
					.addFunction("isActionEnabled", &Input::KeyBinder::isActionEnabled)
					.addFunction("isActionReleased", &Input::KeyBinder::isActionReleased)
					.addFunction("isActionToggled", &Input::KeyBinder::isActionToggled)
					.addFunction("isKeyPressed", &Input::KeyBinder::isKeyPressed)
					.addFunction("loadFromFile", &Input::KeyBinder::loadFromFile)
					.addFunction("setActionDelay", &Input::KeyBinder::setActionDelay)
					.addFunction("setEnabled", &Input::KeyBinder::setEnabled)
					);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.KeyBindImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadLevelSprite(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["LevelSprite"])) (*lua)["Core"]["LevelSprite"] = kaguya::NewTable();
			if (importAll || args[1] == "LevelSprite")
			{
				(*lua)["Core"]["LevelSprite"]["LevelSprite"].setClass(kaguya::UserdataMetatable<Graphics::LevelSprite>()
					.setConstructors<Graphics::LevelSprite(std::string)>()
					.addFunction("addAtr", &Graphics::LevelSprite::addAtr)
					.addFunction("calculateRealCoordinates", &Graphics::LevelSprite::calculateRealCoordinates)
					.addFunction("getAttributes", &Graphics::LevelSprite::getAttributes)
					.addFunction("getH", &Graphics::LevelSprite::getHeight)
					.addFunction("getW", &Graphics::LevelSprite::getWidth)
					.addFunction("getID", &Graphics::LevelSprite::getID)
					.addFunction("getLayer", &Graphics::LevelSprite::getLayer)
					.addFunction("getName", &Graphics::LevelSprite::getPath)
					.addFunction("getOffsetX", &Graphics::LevelSprite::getOffsetX)
					.addFunction("getOffsetY", &Graphics::LevelSprite::getOffsetY)
					.addFunction("getParentID", &Graphics::LevelSprite::getParentID)
					.addFunction("getRect", &Graphics::LevelSprite::getRect)
					.addFunction("getRotation", &Graphics::LevelSprite::getRotation)
					.addFunction("getScaleX", &Graphics::LevelSprite::getScaleX)
					.addFunction("getScaleY", &Graphics::LevelSprite::getScaleY)
					.addFunction("getX", &Graphics::LevelSprite::getX)
					.addFunction("getY", &Graphics::LevelSprite::getY)
					.addFunction("getZDepth", &Graphics::LevelSprite::getZDepth)
					.addFunction("isDrawable", &Graphics::LevelSprite::isDrawable)
					.addFunction("isVisible", &Graphics::LevelSprite::isVisible)
					.addFunction("load", &Graphics::LevelSprite::load)
					.addFunction("move", &Graphics::LevelSprite::move)
					.addFunction("removeAtrByIndex", &Graphics::LevelSprite::removeAtrByIndex)
					.addFunction("removeAtrByName", &Graphics::LevelSprite::removeAtrByName)
					.addFunction("rotate", &Graphics::LevelSprite::rotate)
					.addFunction("scale", &Graphics::LevelSprite::scale)
					.addFunction("setAtr", &Graphics::LevelSprite::setAtr)
					.addFunction("setColor", &Graphics::LevelSprite::setColor)
					.addFunction("setLayer", &Graphics::LevelSprite::setLayer)
					.addFunction("setOffset", &Graphics::LevelSprite::setOffset)
					.addFunction("setParentID", &Graphics::LevelSprite::setParentID)
					.addFunction("setPosition", &Graphics::LevelSprite::setPosition)
					.addFunction("setRotation", &Graphics::LevelSprite::setRotation)
					.addFunction("setRotationOrigin", &Graphics::LevelSprite::setRotationOrigin)
					.addFunction("setScale", &Graphics::LevelSprite::setScale)
					.addFunction("setTranslationOrigin", &Graphics::LevelSprite::setTranslationOrigin)
					.addFunction("setVisible", &Graphics::LevelSprite::setVisible)
					.addFunction("setZDepth", &Graphics::LevelSprite::setZDepth)

					);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.LevelSpriteImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadLight(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Light"])) (*lua)["Core"]["Light"] = kaguya::NewTable();
			if (importAll || args[1] == "PointLight")
			{
				(*lua)["Core"]["Light"]["PointLight"].setClass(kaguya::UserdataMetatable<Light::PointLight>()
					.setConstructors<Light::PointLight(std::string, int, int ,int ,int, double, int, int, int, int, bool)>()
					.addFunction("setPosition", &Light::PointLight::setPosition)
					.addFunction("move", &Light::PointLight::move)
					.addFunction("setOffset", &Light::PointLight::setOffset)
					.addFunction("setSize", &Light::PointLight::setSize)
					.addFunction("scale", &Light::PointLight::scale)
					.addFunction("setColor", &Light::PointLight::setColor)
					.addFunction("addColor", &Light::PointLight::addColor)
					.addFunction("subColor", &Light::PointLight::subColor)
					.addFunction("blendColor", &Light::PointLight::blendColor)
					.addFunction("setBehind", &Light::PointLight::setBehind)
					.addFunction("show", &Light::PointLight::show)
					.addFunction("hide", &Light::PointLight::hide)
					.addFunction("setVisible", &Light::PointLight::setVisible)
					.addFunction("isVisible", &Light::PointLight::isVisible)
					.addFunction("getX", &Light::PointLight::getX)
					.addFunction("getY", &Light::PointLight::getX)
					.addFunction("getSize", &Light::PointLight::getSize)
					.addFunction("getR", &Light::PointLight::getR)
					.addFunction("getG", &Light::PointLight::getG)
					.addFunction("getB", &Light::PointLight::getB)
					.addFunction("getA", &Light::PointLight::getA)
					.addFunction("isBehind", &Light::PointLight::isBehind)
					.addFunction("getType", &Light::PointLight::getType)
					);
				foundPart = true;
			}
			if (importAll || args[1] == "DynamicPointLight")
			{
				(*lua)["Core"]["Light"]["DynamicPointLight"].setClass(
					kaguya::UserdataMetatable<Light::DynamicPointLight, kaguya::MultipleBase<Light::PointLight, Math::MathExpObject>>()
					.setConstructors < Light::DynamicPointLight(std::string, int, int, double)>()
					.addFunction("setSizeExp", &Light::DynamicPointLight::setSizeExp)
					.addFunction("setRExp", &Light::DynamicPointLight::setRExp)
					.addFunction("setGExp", &Light::DynamicPointLight::setGExp)
					.addFunction("setBExp", &Light::DynamicPointLight::setBExp)
					.addFunction("setAExp", &Light::DynamicPointLight::setAExp)
					.addFunction("setOffsetXExp", &Light::DynamicPointLight::setOffsetXExp)
					.addFunction("setOffsetYExp", &Light::DynamicPointLight::setOffsetYExp)
					.addFunction("getType", &Light::DynamicPointLight::getType)
					);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.LightImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadMathExp(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["MathExp"])) (*lua)["Core"]["MathExp"] = kaguya::NewTable();
			if (importAll || args[1] == "MathExp")
			{
				(*lua)["Core"]["MathExp"]["MathExp"].setClass(kaguya::UserdataMetatable<Math::MathExp>()
					.setConstructors<Math::MathExp(std::string, bool)>()
					.addFunction("buildMathExp", &Math::MathExp::buildMathExp)
					.addFunction("getResult", &Math::MathExp::getResult)
					.addFunction("isStatic", &Math::MathExp::isStatic)
					.addFunction("setVar", &Math::MathExp::setVar)
					.addFunction("setExpr", &Math::MathExp::setExpr)
					);
				foundPart = true;
			}
			if (importAll || args[1] == "MathExpObject")
			{
				(*lua)["Core"]["MathExp"]["MathExpObject"].setClass(kaguya::UserdataMetatable<Math::MathExpObject>()
					.setConstructors<Math::MathExpObject(double), Math::MathExpObject(double, std::string, std::string)>()
					.addFunction("processAllExp", &Math::MathExpObject::processAllExp)
					.addFunction("registerMathExp", &Math::MathExpObject::registerMathExp)
					.addFunction("registerOutputVar", &Math::MathExpObject::registerOutputVar)
					.addFunction("setGlobalVar", &Math::MathExpObject::setGlobalVar)
					.addFunction("setLocalVar", &Math::MathExpObject::setLocalVar)
					.addFunction("setPrecision", &Math::MathExpObject::setPrecision)
					.addFunction("getPrecision", &Math::MathExpObject::getPrecision)
					);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.MathExpImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadPackage(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Package"])) (*lua)["Core"]["Package"] = kaguya::NewTable();
			if (importAll || args[1] == "Package")
			{
				(*lua)["Core"]["Package"]["Package"].setClass(kaguya::UserdataMetatable<System::Package>()
					.addStaticFunction("Load", &System::Package::Load)
					.addStaticFunction("Install", &System::Package::Install)
					.addStaticFunction("GetPackageLocation", &System::Package::GetPackageLocation)
					.addStaticFunction("PackageExists", &System::Package::PackageExists)
				);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.PackageImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadPath(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Path"])) (*lua)["Core"]["Path"] = kaguya::NewTable();
			if (importAll || args[1] == "PriorizedPath")
			{
				(*lua)["Core"]["Path"]["PriorizedPath"].setClass(kaguya::UserdataMetatable<System::PriorizedPath>()
					.addFunction("getPath", &System::PriorizedPath::getPath)
				);
				foundPart = true;
			}
			if (importAll || args[1] == "Path")
			{
				(*lua)["Core"]["Path"]["Path"].setClass(kaguya::UserdataMetatable<System::Path>()
					.addStaticFunction("Paths", &System::Path::Paths)
				);
				foundPart = true;
			}
			(*lua)["Core"]["Path"]["MountPaths"] = kaguya::function(System::MountPaths);
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.PathImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadParticle(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Particle"])) (*lua)["Core"]["Particle"] = kaguya::NewTable();
			if (importAll || args[1] == "Particle")
			{
				(*lua)["Core"]["Particle"]["Particle"].setClass(kaguya::UserdataMetatable<Graphics::Particle>()
					.addFunction("getAngle", &Graphics::Particle::getAngle)
					.addFunction("getLifetime", &Graphics::Particle::getLifetime)
					.addFunction("getParticle", &Graphics::Particle::getParticle)
					.addFunction("getVelocity", &Graphics::Particle::getVelocity)
					.addFunction("init", &Graphics::Particle::init)
					.addFunction("isDead", &Graphics::Particle::isDead)
					.addFunction("setAngle", &Graphics::Particle::setAngle)
					.addFunction("setEndColor", &Graphics::Particle::setEndColor)
					.addFunction("setLifetime", &Graphics::Particle::setLifetime)
					.addFunction("setStartColor", &Graphics::Particle::setStartColor)
					.addFunction("setVelocity", &Graphics::Particle::setVelocity)
					.addFunction("shapeInit", &Graphics::Particle::shapeInit)
					.addFunction("update", &Graphics::Particle::update)
				);
				foundPart = true;
			}
			if (importAll || args[1] == "MathParticle")
			{
				(*lua)["Core"]["Particle"]["MathParticle"].setClass(kaguya::UserdataMetatable<Graphics::MathParticle, Math::MathExpObject>()
					.setConstructors<Graphics::MathParticle(), 
					Graphics::MathParticle(std::string, std::string, std::string, std::string, std::string, std::string, std::string, double),
					Graphics::MathParticle(std::string)>()
					.addFunction("setExp", &Graphics::MathParticle::setExp)
					.addFunction("update", &Graphics::MathParticle::update)
				);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.ParticleImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadSFML(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["SFML"])) (*lua)["Core"]["SFML"] = kaguya::NewTable();
			if (importAll || args[1] == "Color")
			{
				(*lua)["Core"]["SFML"]["Color"].setClass(kaguya::UserdataMetatable<sf::Color>()
					.setConstructors<sf::Color(int, int, int, int)>()
					.addFunction("a", &sf::Color::a)
					.addFunction("r", &sf::Color::r)
					.addFunction("g", &sf::Color::b)
					.addFunction("b", &sf::Color::b)
					);
				foundPart = true;
			}
			if (importAll || args[1] == "Drawable")
			{
				(*lua)["Core"]["SFML"]["Drawable"].setClass(kaguya::UserdataMetatable<sf::Drawable>()
					);
				foundPart = true;
			}
			if (importAll || args[1] == "Transformable")
			{
				(*lua)["Core"]["SFML"]["Transformable"].setClass(kaguya::UserdataMetatable<sf::Transformable>()
					.addFunction("getInverseTransform", &sf::Transformable::getInverseTransform)
					.addFunction("getOrigin", &sf::Transformable::getOrigin)
					.addFunction("getPosition", &sf::Transformable::getPosition)
					.addFunction("getRotation", &sf::Transformable::getRotation)
					.addFunction("getScale", &sf::Transformable::getScale)
					.addFunction("getTransform", &sf::Transformable::getTransform)
					.addOverloadedFunctions("move",
						static_cast<void (sf::Transformable::*)(float, float)>(&sf::Transformable::move),
						static_cast<void (sf::Transformable::*)(const sf::Vector2f&)>(&sf::Transformable::move)
					)
					.addFunction("rotate", &sf::Transformable::rotate)
					.addOverloadedFunctions("scale",
						static_cast<void (sf::Transformable::*)(float, float)>(&sf::Transformable::scale),
						static_cast<void (sf::Transformable::*)(const sf::Vector2f&)>(&sf::Transformable::scale)
					)
					.addOverloadedFunctions("setOrigin", 
						static_cast<void (sf::Transformable::*)(float, float)>(&sf::Transformable::setOrigin),
						static_cast<void (sf::Transformable::*)(const sf::Vector2f&)>(&sf::Transformable::setOrigin)
					)
					);
				foundPart = true;
			}
			if (importAll || args[1] == "Sprite")
			{
				(*lua)["Core"]["SFML"]["Sprite"].setClass(kaguya::UserdataMetatable<sf::Sprite, kaguya::MultipleBase<sf::Drawable, sf::Transformable>>()
					);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.SFMLImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}

		void CoreLib::loadSound(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Sound"])) (*lua)["Core"]["Sound"] = kaguya::NewTable();
			if (importAll || args[1] == "Sound")
			{
				(*lua)["Core"]["Sound"]["Sound"].setClass(kaguya::UserdataMetatable<Sound::SoundWrapper>()
					.setConstructors<Sound::SoundWrapper(), Sound::SoundWrapper(const std::string&)>()
					.addFunction("doesUseSoundPosition", &Sound::SoundWrapper::doesUsesSoundPosition)
					.addFunction("getMinimumDistance", &Sound::SoundWrapper::getMinimumDistance)
					.addFunction("getPitch", &Sound::SoundWrapper::getPitch)
					.addFunction("getPlayingOffset", &Sound::SoundWrapper::getPlayingOffset)
					.addFunction("getPosition", &Sound::SoundWrapper::getPosition)
					.addFunction("getSpatialAttenuation", &Sound::SoundWrapper::getSpatialAttenuation)
					.addFunction("getStatus", &Sound::SoundWrapper::getStatus)
					.addFunction("getVolume", &Sound::SoundWrapper::getVolume)
					.addFunction("isLooping", &Sound::SoundWrapper::isLooping)
					.addFunction("load", &Sound::SoundWrapper::load)
					.addFunction("pause", &Sound::SoundWrapper::pause)
					.addFunction("play", &Sound::SoundWrapper::play)
					.addFunction("setLooping", &Sound::SoundWrapper::setLooping)
					.addFunction("setMinimumDistance", &Sound::SoundWrapper::setMinimumDistance)
					.addFunction("setPitch", &Sound::SoundWrapper::setPitch)
					.addFunction("setPlayingOffset", &Sound::SoundWrapper::setPlayingOffset)
					.addFunction("setPosition", &Sound::SoundWrapper::setPosition)
					.addFunction("setSpatialAttenuation", &Sound::SoundWrapper::setSpatialAttenuation)
					.addFunction("setVolume", &Sound::SoundWrapper::setVolume)
					.addFunction("stop", &Sound::SoundWrapper::stop)
					.addFunction("useSoundPosition", &Sound::SoundWrapper::useSoundPosition)
				);
				foundPart = true;
			}
			if (importAll || args[1] == "Sound")
			{
				(*lua)["Core"]["Sound"]["Music"].setClass(kaguya::UserdataMetatable<Sound::MusicWrapper>()
					.setConstructors<Sound::MusicWrapper(), Sound::MusicWrapper(const std::string&)>()
					.addFunction("doesUseSoundPosition", &Sound::MusicWrapper::doesUsesSoundPosition)
					.addFunction("getMinimumDistance", &Sound::MusicWrapper::getMinimumDistance)
					.addFunction("getPitch", &Sound::MusicWrapper::getPitch)
					.addFunction("getPlayingOffset", &Sound::MusicWrapper::getPlayingOffset)
					.addFunction("getPosition", &Sound::MusicWrapper::getPosition)
					.addFunction("getSpatialAttenuation", &Sound::MusicWrapper::getSpatialAttenuation)
					.addFunction("getStatus", &Sound::MusicWrapper::getStatus)
					.addFunction("getVolume", &Sound::MusicWrapper::getVolume)
					.addFunction("isLooping", &Sound::MusicWrapper::isLooping)
					.addFunction("load", &Sound::MusicWrapper::load)
					.addFunction("pause", &Sound::MusicWrapper::pause)
					.addFunction("play", &Sound::MusicWrapper::play)
					.addFunction("setLooping", &Sound::MusicWrapper::setLooping)
					.addFunction("setMinimumDistance", &Sound::MusicWrapper::setMinimumDistance)
					.addFunction("setPitch", &Sound::MusicWrapper::setPitch)
					.addFunction("setPlayingOffset", &Sound::MusicWrapper::setPlayingOffset)
					.addFunction("setPosition", &Sound::MusicWrapper::setPosition)
					.addFunction("setSpatialAttenuation", &Sound::MusicWrapper::setSpatialAttenuation)
					.addFunction("setVolume", &Sound::MusicWrapper::setVolume)
					.addFunction("stop", &Sound::MusicWrapper::stop)
					.addFunction("useSoundPosition", &Sound::MusicWrapper::useSoundPosition)
				);
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.SoundImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}

		void CoreLib::loadSTD(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["STD"])) (*lua)["Core"]["STD"] = kaguya::NewTable();
			if (importAll || args[1] == "Pair")
			{
				(*lua)["Core"]["STD"]["IntPair"].setClass(kaguya::UserdataMetatable<std::pair<int, int>>()
					.setConstructors<std::pair<int, int>(int, int)>()
					.addFunction("first", &std::pair<int, int>::first)
					.addFunction("second", &std::pair<int, int>::second)
					.addFunction("swap", &std::pair<int, int>::swap)
					);
				(*lua)["Core"]["STD"]["DoublePair"].setClass(kaguya::UserdataMetatable<std::pair<double, double>>()
					.setConstructors<std::pair<double, double>(double, double)>()
					.addFunction("first", &std::pair<double, double>::first)
					.addFunction("second", &std::pair<double, double>::second)
					.addFunction("swap", &std::pair<double, double>::swap)
					);
				foundPart = true;
			}
			if (importAll || args[1] == "Print")
			{
				(*lua)["Core"]["STD"]["Print"] = kaguya::function([](std::string disp) { std::cout << "[Lua] : " << disp << std::endl; });
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.STDImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}

		void CoreLib::loadTrigger(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Trigger"])) (*lua)["Core"]["Trigger"] = kaguya::NewTable();
			if (importAll || args[1] == "TriggerDatabase")
			{
				(*lua)["Core"]["Trigger"]["TriggerDatabase"].setClass(kaguya::UserdataMetatable<TriggerDatabase>()
					.addFunction("getTrigger", &TriggerDatabase::getTrigger)
					.addFunction("createTriggerGroup", &TriggerDatabase::createTriggerGroup)
					.addFunction("joinTriggerGroup", &TriggerDatabase::joinTriggerGroup)
					.addFunction("doesTriggerGroupExists", &TriggerDatabase::doesTriggerGroupExists)
					);
				foundPart = true;
			}
			if (importAll || args[1] == "TriggerGroup")
			{
				(*lua)["Core"]["Trigger"]["TriggerGroup"].setClass(kaguya::UserdataMetatable<TriggerGroup>()
					.addFunction("getTrigger", &TriggerGroup::getTrigger)
					.addFunction("addTrigger", &TriggerGroup::addTrigger)
					.addFunction("delayTriggerState", &TriggerGroup::delayTriggerState)
					.addFunction("enableTrigger", &TriggerGroup::enableTrigger)
					.addFunction("disableTrigger", &TriggerGroup::disableTrigger)
					.addFunction("setTriggerState", &TriggerGroup::setTriggerState)
					.addFunction("getState", &TriggerGroup::getState)
					.addFunction("setPermanent", &TriggerGroup::setPermanent)
					.addFunction("getAllTriggersName", &TriggerGroup::getAllTriggersName)
					.addFunction("getAllTriggers", &TriggerGroup::getAllTriggers)
					.addOverloadedFunctions("pushParameter",
						&TriggerGroup::pushParameter<int>,
						&TriggerGroup::pushParameter<float>,
						&TriggerGroup::pushParameter<std::string>,
						&TriggerGroup::pushParameter<bool>,
						&TriggerGroup::pushParameter<std::vector<int>>,
						&TriggerGroup::pushParameter<std::vector<float>>,
						&TriggerGroup::pushParameter<std::vector<std::string>>,
						&TriggerGroup::pushParameter<std::vector<bool>>,
						&TriggerGroup::pushParameter<std::map<int, int>>,
						&TriggerGroup::pushParameter<std::map<int, float>>,
						&TriggerGroup::pushParameter<std::map<int, std::string>>,
						&TriggerGroup::pushParameter<std::map<int, bool>>,
						&TriggerGroup::pushParameter<std::map<std::string, int>>,
						&TriggerGroup::pushParameter<std::map<std::string, float>>,
						&TriggerGroup::pushParameter<std::map<std::string, std::string>>,
						&TriggerGroup::pushParameter<std::map<std::string, bool>>
					)
				);
				foundPart = true;
			}
			if (importAll || args[1] == "Trigger")
			{
				(*lua)["Core"]["Trigger"]["Trigger"].setClass(kaguya::UserdataMetatable<Trigger>()
					.addFunction("getState", &Trigger::getState)
					.addFunction("isPermanent", &Trigger::isPermanent)
					.addFunction("getGroup", &Trigger::getGroup)
					.addFunction("getName", &Trigger::getName)
					.addFunction("getNamespace", &Trigger::getNamespace)
					);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.TriggerImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadUtils(kaguya::State* lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Utils"])) (*lua)["Core"]["Utils"] = kaguya::NewTable();
			if (importAll || args[1] == "File")
			{
				if (!static_cast<bool>((*lua)["Core"]["Utils"]["File"])) (*lua)["Core"]["Utils"]["File"] = kaguya::NewTable();
				(*lua)["Core"]["Utils"]["File"]["listDirInDir"] = kaguya::function(Functions::File::listDirInDir);
				(*lua)["Core"]["Utils"]["File"]["listFileInDir"] = kaguya::function(Functions::File::listFileInDir);
				(*lua)["Core"]["Utils"]["File"]["copy"] = kaguya::function(Functions::File::copy);
				(*lua)["Core"]["Utils"]["File"]["separator"] = kaguya::function(Functions::File::separator);
				foundPart = true;
			}
			if (importAll || args[1] == "Math")
			{
				if (!static_cast<bool>((*lua)["Core"]["Utils"]["Math"])) (*lua)["Core"]["Utils"]["Math"] = kaguya::NewTable();
				(*lua)["Core"]["Utils"]["Math"]["randint"] = kaguya::function(Functions::Math::randint);
				(*lua)["Core"]["Utils"]["Math"]["randfloat"] = kaguya::function(Functions::Math::randfloat);
				(*lua)["Core"]["Utils"]["Math"]["getMin"] = kaguya::function(Functions::Math::getMin<double>);
				(*lua)["Core"]["Utils"]["Math"]["getMax"] = kaguya::function(Functions::Math::getMax<double>);
				(*lua)["Core"]["Utils"]["Math"]["isBetween"] = kaguya::function(Functions::Math::isBetween<double>);
				(*lua)["Core"]["Utils"]["Math"]["isDoubleInt"] = kaguya::function(Functions::Math::isDoubleInt);
				foundPart = true;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.UtilsImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
		void CoreLib::loadVili(kaguya::State * lua, std::vector<std::string> args)
		{
			registerLib(lua, Functions::Vector::join(args, "."));
			bool importAll = args.size() == 1;
			bool foundPart = false;
			if (!static_cast<bool>((*lua)["Core"]["Vili"])) (*lua)["Core"]["Vili"] = kaguya::NewTable();
			if (importAll || args[1] == "DataParser")
			{
				(*lua)["Core"]["Vili"]["DataParser"].setClass(kaguya::UserdataMetatable<vili::DataParser>()
					.setConstructors<vili::DataParser(), vili::DataParser(std::string)>()
					.addFunction("createFlag", &vili::DataParser::createFlag)
					.addFunction("root", &vili::DataParser::operator->)
					//.addFunction("at", &vili::DataParser::at)
					.addFunction("getAmountOfFlags", &vili::DataParser::getAmountOfFlags)
					.addFunction("getFlagAtIndex", &vili::DataParser::getFlagAtIndex)
					.addFunction("hasFlag", &vili::DataParser::hasFlag)
					.addFunction("parseFile", &vili::DataParser::parseFile)
					.addFunction("writeFile", &vili::DataParser::writeFile)
				);
				foundPart = true;
			}
			if (importAll || Functions::String::contains(args[1], "Attribute")) {
				(*lua)["Core"]["Vili"]["Attribute"].setClass(kaguya::UserdataMetatable<vili::Attribute>()
					.addFunction("getID", &vili::Attribute::getID)
					.addFunction("getType", &vili::Attribute::getType)
				);
				foundPart = true;
			}
			if (importAll || args[1] == "ComplexAttribute")
			{
				(*lua)["Core"]["Vili"]["ComplexAttribute"].setClass(kaguya::UserdataMetatable<vili::ComplexAttribute, vili::Attribute>()
					.setConstructors<
					vili::ComplexAttribute(std::string),
					vili::ComplexAttribute(std::string, vili::ComplexAttribute*),
					vili::ComplexAttribute(std::string, std::vector<vili::ComplexAttribute*>*)>()
					.addFunction("at", &vili::ComplexAttribute::getPath)
					.addOverloadedFunctions("createBaseAttribute",
						static_cast<void (vili::ComplexAttribute::*)(const std::string&, const vili::Types::DataType&, const std::string&)>(&vili::ComplexAttribute::createBaseAttribute),
						static_cast<void (vili::ComplexAttribute::*)(const std::string&, int)>(&vili::ComplexAttribute::createBaseAttribute),
						static_cast<void (vili::ComplexAttribute::*)(const std::string&, double)>(&vili::ComplexAttribute::createBaseAttribute),
						static_cast<void (vili::ComplexAttribute::*)(const std::string&, bool)>(&vili::ComplexAttribute::createBaseAttribute),
						static_cast<void (vili::ComplexAttribute::*)(const std::string&, const std::string&)>(&vili::ComplexAttribute::createBaseAttribute)
					)
					.addOverloadedFunctions("contains",
						static_cast<bool (vili::ComplexAttribute::*)(const std::string&)>(&vili::ComplexAttribute::contains),
						static_cast<bool (vili::ComplexAttribute::*)(vili::Types::AttributeType, const std::string&)>(&vili::ComplexAttribute::contains)
					)
					.addFunction("createComplexAttribute", &vili::ComplexAttribute::createComplexAttribute)
					.addFunction("createListAttribute", &vili::ComplexAttribute::createListAttribute)
					.addFunction("deleteBaseAttribute", &vili::ComplexAttribute::deleteBaseAttribute)
					.addFunction("deleteComplexAttribute", &vili::ComplexAttribute::deleteComplexAttribute)
					.addFunction("deleteListAttribute", &vili::ComplexAttribute::deleteListAttribute)
					.addFunction("getAll", &vili::ComplexAttribute::getAll)
					.addFunction("getBaseAttribute", &vili::ComplexAttribute::getBaseAttribute)
					.addFunction("getComplexAttribute", &vili::ComplexAttribute::getComplexAttribute)
					.addFunction("getListAttribute", &vili::ComplexAttribute::getListAttribute)
					.addFunction("getPath", &vili::ComplexAttribute::getPath)
					.addFunction("heritage", &vili::ComplexAttribute::heritage)
					.addFunction("pushBaseAttribute", &vili::ComplexAttribute::pushBaseAttribute)
					.addFunction("pushComplexAttribute", &vili::ComplexAttribute::pushComplexAttribute)
					.addFunction("pushListAttribute", &vili::ComplexAttribute::pushListAttribute)
					.addFunction("write", &vili::ComplexAttribute::write)
				);
				foundPart = true;
			}
			if (importAll || args[1] == "ListAttribute")
			{
				(*lua)["Core"]["Vili"]["ListAttribute"].setClass(kaguya::UserdataMetatable<vili::ListAttribute, vili::Attribute>()
					.setConstructors<vili::ListAttribute(std::string)>()
					.addOverloadedFunctions("push",
						static_cast<void (vili::ListAttribute::*)(const int&)>(&vili::ListAttribute::push),
						static_cast<void (vili::ListAttribute::*)(const bool&)>(&vili::ListAttribute::push),
						static_cast<void (vili::ListAttribute::*)(const double&)>(&vili::ListAttribute::push),
						static_cast<void (vili::ListAttribute::*)(const std::string&)>(&vili::ListAttribute::push)
					)
					.addFunction("get", &vili::ListAttribute::get)
					.addFunction("clear", &vili::ListAttribute::clear)
					.addOverloadedFunctions("insert",
						static_cast<void (vili::ListAttribute::*)(unsigned int, const int&)>(&vili::ListAttribute::insert),
						static_cast<void (vili::ListAttribute::*)(unsigned int, const bool&)>(&vili::ListAttribute::insert),
						static_cast<void (vili::ListAttribute::*)(unsigned int, const double&)>(&vili::ListAttribute::insert),
						static_cast<void (vili::ListAttribute::*)(unsigned int, const std::string&)>(&vili::ListAttribute::insert)
					)
					.addFunction("erase", &vili::ListAttribute::erase)
					.addFunction("size", &vili::ListAttribute::size)
				);
				foundPart = true;
			}
			if (importAll || args[1] == "BaseAttribute")
			{
				(*lua)["Core"]["Vili"]["BaseAttribute"].setClass(kaguya::UserdataMetatable<vili::BaseAttribute, vili::Attribute>()
					.setConstructors<vili::BaseAttribute(std::string, vili::Types::DataType)>()
					.addFunction("get_int", static_cast<int(vili::BaseAttribute::*)()>(&vili::BaseAttribute::get))
					.addFunction("get_float", static_cast<double(vili::BaseAttribute::*)()>(&vili::BaseAttribute::get))
					.addFunction("get_bool", static_cast<bool(vili::BaseAttribute::*)()>(&vili::BaseAttribute::get))
					.addFunction("get_string", static_cast<std::string(vili::BaseAttribute::*)()>(&vili::BaseAttribute::get))
					.addFunction("getDataType", &vili::BaseAttribute::getDataType)
					.addFunction("returnData", &vili::BaseAttribute::returnData)
					.addOverloadedFunctions("set",
						static_cast<void (vili::BaseAttribute::*)(int)>(&vili::BaseAttribute::set),
						static_cast<void (vili::BaseAttribute::*)(double)>(&vili::BaseAttribute::set),
						static_cast<void (vili::BaseAttribute::*)(bool)>(&vili::BaseAttribute::set),
						static_cast<void (vili::BaseAttribute::*)(const std::string&)>(&vili::BaseAttribute::set)
					)
				);
				foundPart = true;
			}
			if (importAll || args[1] == "AttributeType") {
				(*lua)["Core"]["Vili"]["AttributeType"] = kaguya::NewTable();
				(*lua)["Core"]["Vili"]["AttributeType"]["BaseAttribute"] = vili::Types::BaseAttribute;
				(*lua)["Core"]["Vili"]["AttributeType"]["ComplexAttribute"] = vili::Types::ComplexAttribute;
				(*lua)["Core"]["Vili"]["AttributeType"]["ListAttribute"] = vili::Types::ListAttribute;
			}
			if (!foundPart) throw aube::ErrorHandler::Raise("ObEngine.Script.Lib.ViliImportError", { { "lib", Functions::Vector::join(args, ".") } });
		}
	}
}