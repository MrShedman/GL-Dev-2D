
#include "GameState.hpp"
#include "..\rendering\RenderTarget2D.h"
#include "..\rendering\Shader.hpp"
#include "..\resource management\ResourceHolder.hpp"
#include "..\system\Utilities.h"

#include "..\experiment\Material.h"
#include "..\experiment\Composite.h"
#include "..\experiment\Laminate.h"
#include "..\experiment\RobotArm.h"

namespace
{
	const double pi = 4 * std::atan(1.0);
}


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

	Block b;
	b.create(300);
	b.setPosition(0, -300, 0);
	b.setTexture(context.textures->get(Textures::Texture1));
	mBlocks.push_back(std::move(b));
	
	Graph graph1;

	graph1.setEquation([](float x, float y)
	{			
		Material fibre(72.0f, 0.22f);
			
		Material matrix(2.4f, 0.34f);
			
		Composite composite(fibre, matrix, x);
			
		Laminate::Properties prop = Laminate::getProperties(composite, 0.1f, { 0.0f, y });
			
		return prop.Ex;
	});
		
	graph1.setRange(Range(0.0f, 0.02f, 1.0f), Range(0.0f, 2.0f, 90.0f));
	graph1.setSize(Vector3f(100.f, 50.f, 100.f));
	graph1.plot();
	

//	graph1.setPalette(Graph::Palette::Jet);
////	graph1.setPosition(200, 200, 100);
//	graph1.setSize(Vector3f(100.f, 50.f, 100.f));

	Graph graph2;

	Image i;
	i.loadFromFile("res/textures/heightmap.png");
	auto s = i.getSize();
	graph2.setEquation([&i](float x, float y)
	{
		return i.getPixel(x, y).r;
	});

	graph2.setRange(Range(0, 1, s.x-1), Range(0, 1, s.y-1));
	graph2.setPalette(Graph::Palette::Jet);
	//graph2.setPosition(200, 200, 200);
	graph2.setSize(Vector3f(100.f, 10.f, 100.f));
	graph2.showWireframe(true);
	graph2.plot();
	/*
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
	*/

	m_graphs.push_back(std::move(graph1));
	//m_graphs.push_back(std::move(graph2));
	//m_graphs.push_back(std::move(graph3));
	//m_graphs.push_back(std::move(graph4));

	Mesh mesh1("Stand.stl", Color(150, 150, 150));
	
	Mesh mesh2("Stand2.stl", Color(150, 150, 150));

	m_meshes.push_back(std::move(mesh1));
	m_meshes.push_back(std::move(mesh2));

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

	glEnable(GL_POLYGON_OFFSET_LINE);
	
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glPolygonOffset(-1, -1);

	//glEnable(GL_CULL_FACE); // cull face
	//glCullFace(GL_BACK); // cull back face
	//glFrontFace(GL_CW);
	
	states.cam = &cam;
	target.draw(m_robot, states);
	Shader2D* s = &states.shaderHolder->get(Shaders::Lighting);

	s->setParameter("light.position", states.cam->getPosition());
	s->setParameter("light.intensities", 1.f, 1.f, 1.f);
	states.shader = s;

	for (auto &m : m_meshes)
	{
		target.draw(m, states);
	}


	//for (auto &b : mBlocks)
	//{
	//	target.draw(b, states);
	//}

	states.shader = &states.shaderHolder->get(Shaders::Default);

	/*for (auto & g : m_graphs)
	{
		g.showWireframe(polygonMode);
		target.draw(g, states);
	}*/

	glDisable(GL_DEPTH_TEST);

	target.draw(mGUIContainer, states);
	states.shader = &states.shaderHolder->get(Shaders::SDFF);
	target.draw(mText, states);
}

bool GameState::update(Time dt)
{
	Window& window = *getContext().window;
	mGUIContainer.update();
	cam.Update(dt.asSeconds());

	m_robot.rotate(Vector3f(0.0f, 0.0f, 0.f), Vector3f(0.0f, 1.0f, 0.f), speed*dt.asSeconds());

	/*for (auto &g : m_graphs)
	{
		g.rotate(g.getOrigin(), Vector3f(0.0f, 1.0f, 0.f), m*speed*dt.asSeconds());
	}*/

	return true;
}

bool GameState::handleEvent(const Event& event)
{
	if (!mGUIContainer.handleEvent(event))
	{
		cam.handleEvent(event);
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

	if (event.type == Event::FileDrop)
	{
		int size = event.fileDrop.fileCount;

		std::cout << size << " files dropped at: " << event.fileDrop.x << ", " << event.fileDrop.y << std::endl;

		if (size == 1)
		{
			Mesh mesh(event.fileDrop.files[0], Color(150, 150, 150), true);
			float x = randomize(0, 200);
			float z = randomize(0, 200);
			mesh.setPosition(x - 100, 0, z - 100);

			m_meshes.push_back(std::move(mesh));
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

	for (int i = -50; i <= 50; ++i)
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
