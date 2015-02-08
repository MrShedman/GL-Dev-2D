
#include "GameState.hpp"
#include "..\rendering\RenderTarget2D.h"
#include "..\resource management\ResourceHolder.hpp"
#include "..\system\Utilities.h"

#include "..\experiment\Material.h"
#include "..\experiment\Composite.h"
#include "..\experiment\Laminate.h"

namespace
{
	const double pi = 4 * std::atan(1.0);
}
//graph([](double x, double z)
//{
//	return std::cos(std::abs(x) + std::abs(z));
//	//return x * z * std::exp(-(x*x) - (z * z));
//	double R = std::sqrt(x * x + z*z) + DBL_EPSILON;
//	return std::sin(R) / R;
//},
//Range(-4, 0.1, 4), Range(-4, 0.1, 4)
//)
////graph([](double x, double z)
////{			
////	return 1.0 - std::cosh(2 * x*z) / std::cosh(x);
////},
////Range(-0.5, 0.1, 0.5), Range(0, 0.1, 1)
////)
////graph([](int x, int z)
////{			
////Material fibre(76, 0.22);
////
////Material matrix(2.4, 0.34);
////
////Composite composite(fibre, matrix, double(x) / 100.0);
////
////Laminate::Properties prop = Laminate::getProperties(composite, 0.13, { 0, double(z) });
////
////return prop.vxy;
////}, 100, 90
////)


GameState::GameState(StateStack& stack, Context context)
:
State(stack, context),
mGUIContainer(),
speed(1.0f),
mText("test@;#£$%", context.fonts->get(Fonts::Main)),
mBlocks(),
m_graphs(),
polygonMode(false)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::MenuBackground));

	Util::scaleSpriteToFitWindow(mBackgroundSprite, *context.window);

	mText.renderMesh();
	mText.setColor(Color::Magenta);
	mText.setPosition(900, 100);

	cam.init(context.window);

	Graph graph1;

	graph1.setEquation([](float x, float z)
	{			
		Material fibre(76.0f, 0.22f);
			
		Material matrix(2.4f, 0.34f);
			
		Composite composite(fibre, matrix, x);
			
		Laminate::Properties prop = Laminate::getProperties(composite, 0.13f, { 0.0f, z });
			
		return prop.vxy;		
	});

	graph1.setRange(Range(0.0f, 0.01f, 1.0f ), Range(0.0f, 1.0f, 90.0f));
	graph1.setPalette(Graph::Palette::Jet);

	Graph graph2;

	graph2.setEquation([](float x, float z)
	{
		return std::sin(x) * std::cos(z);
	});

	graph2.setRange(Range(-6.0f, 0.1f, 6.0f), Range(-6.0f, 0.1f, 6.0f));
	graph2.setPalette(Graph::Palette::Hot);
	graph2.setPosition(Vector3f(0.0f, 0.0f, 100.0f));

	Graph graph3;

	graph3.setEquation([](float x, float z)
	{
		float R = std::sqrt(x * x + z * z) + FLT_EPSILON;
		return std::sin(R) / R;
	});

	graph3.setRange(Range(-8.0f, 0.1f, 8.0f), Range(-8.0f, 0.1f, 8.0f));
	graph3.setPalette(Graph::Palette::Cool);
	graph3.setPosition(Vector3f(100.0f, 0.0f, 100.0f));

	Graph graph4;

	graph4.setEquation([](float x, float z)
	{
		return std::cos(std::abs(x) + std::abs(z));
	});

	graph4.setRange(Range(-2.0f, 0.1f, 2.0f), Range(-2.0f, 0.1f, 2.0f));
	graph4.setPalette(Graph::Palette::Gray);
	graph4.setPosition(Vector3f(100.0f, 0.0f, 0.0f));

	m_graphs.push_back(std::move(graph1));
	m_graphs.push_back(std::move(graph2));
	m_graphs.push_back(std::move(graph3));
	m_graphs.push_back(std::move(graph4));

	for (auto &g : m_graphs)
	{
		g.setSize(Vector3f(100.0f, 50.0f, 100.0f));
		g.plot();
	}

	initializeButtons();
}

#include "..\math\Frustum.hpp"
void GameState::draw()
{
	RenderTarget2D& target = *getContext().target;

	RenderStates states;
	states.shaderHolder = getContext().shaders;
	states.shader = &states.shaderHolder->get(Shaders::Default);

	//target.draw(mBackgroundSprite, states);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDepthRange(0.0f, 1.0f);

	//glEnable(GL_CULL_FACE); // cull face
	//glCullFace(GL_BACK); // cull back face
	//glFrontFace(GL_CW);

	states.cam = &cam;
	
	if (polygonMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	for (auto & g : m_graphs)
	{
		target.draw(g, states);
	}

	if (polygonMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	target.draw(mGUIContainer, states);
	states.shader = &states.shaderHolder->get(Shaders::SDFF);
	target.draw(mText, states);
}

static float m = 1.0f;

bool GameState::update(Time dt)
{
	Window& window = *getContext().window;
	mGUIContainer.update();
	cam.Update(dt.asSeconds());

	for (auto &g : m_graphs)
	{
		g.rotate(g.getOrigin(), Vector3f(0.0f, 1.0f, 0.f), m*speed*dt.asSeconds());
	}

	return true;
}

bool GameState::handleEvent(const Event& event)
{
	if (!mGUIContainer.handleEvent(event))
	{
		cam.handleEvent(event, 1.0f);
	}
	
	if (event.type == Event::Resized)
	{
		//camera.setViewSize(static_cast<sf::Vector2f>(getContext().window->getSize()));
	}
	if (event.type == Event::KeyPressed)
	{
		// Escape pressed, trigger the pause screen
		if (event.key.code == Keyboard::Escape)
		{
			requestStackPop();
			requestStackPush(States::Menu);
		}
		if (event.key.code == Keyboard::P)
		{
			polygonMode = !polygonMode;
		}
	}
	if (event.type == Event::MouseWheelMoved)
	{
		m += event.mouseWheel.delta / 100.f;
	}


	Window& window = *getContext().window;

	if (event.type == Event::KeyPressed)
	{
		if (event.key.code == Keyboard::Return && event.key.alt)
		{
			if (window.isFullscreen())
			{
				window.switchToWindowed(VideoMode(1280, 720), Style::Default, true);
			}
			else
			{
				window.switchToFullscreen(VideoMode::getFullscreenModes()[0]);
			}
		}
	}
	
	return true;
}

void GameState::initializeButtons()
{
	mGUIContainer.clear();

	float y = 0.5f * getContext().window->getSize().y;
	float x = 0.5f * getContext().window->getSize().x;

	std::vector<float> scaleSpeed;

	for (int i = 0; i <= 100; ++i)
	{
		scaleSpeed.push_back(float(i) * 0.1f);
	}

	auto resolution = std::make_shared<GUI::Slider<float>>(getContext());
	resolution->setSize(Vector2f(250, 45));
	resolution->setPosition(Vector2f(20, 20));
	resolution->setPossibleValues(scaleSpeed);
	resolution->setCurrentValue(1.f);
	resolution->setDisplayFunction([](float d)
	{
		return "Speed: " + Util::toString(d);
	});
	resolution->setCallback([this, scaleSpeed](int id)
	{
		speed = scaleSpeed[id];
	});

	mGUIContainer.pack(resolution);
}
