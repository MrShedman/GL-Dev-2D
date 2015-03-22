
#include "GameState.hpp"
#include "..\rendering\RenderTarget2D.h"
#include "..\rendering\Shader.hpp"
#include "..\resource management\ResourceHolder.hpp"
#include "..\system\Utilities.h"

#include "..\experiment\Material.h"
#include "..\experiment\Composite.h"
#include "..\experiment\Laminate.h"
#include "..\experiment\RobotArm.h"
#include "..\experiment\MousePicker.h"
#include "..\experiment\Bolt.hpp"

namespace
{
	//const float pi = 4 * std::atan(1.0);

	void setParameter(Shader2D* s, const PointLight& light)
	{
		s->setParameter("gSpecColor", Vector3f(1.0f, 0.4f, 0.0f));
		s->setParameter("gPointLight.Base.Color", light.Color);
		s->setParameter("gPointLight.Base.AmbientIntensity", light.AmbientIntensity);
		s->setParameter("gPointLight.Position", light.Position);
		s->setParameter("gPointLight.Base.DiffuseIntensity", light.DiffuseIntensity);
		s->setParameter("gPointLight.Atten.Constant", light.Attenuation.Constant);
		s->setParameter("gPointLight.Atten.Linear", light.Attenuation.Linear);
		s->setParameter("gPointLight.Atten.Exp", light.Attenuation.Exp);
	}
}


GameState::GameState(StateStack& stack, Context context)
:
State(stack, context),
mGUIContainer(),
speed(1.0f),
mText("test@;#£$%", context.fonts->get(Fonts::Main)),
mBlocks(),
m_graphs(),
polygonMode(false),
m_robot()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::MenuBackground));

	Util::scaleSpriteToFitWindow(mBackgroundSprite, *context.window);

	mText.renderMesh();
	mText.setColor(Color::Magenta);
	mText.setPosition(900, 100);

	cam.init(context.window);
	
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
	graph1.setPalette(Graph::Palette::Jet);
	
	Graph graph2;

	Image i;
	i.loadFromFile("res/textures/heightmap4.png");
	auto s = i.getSize();
	graph2.setEquation([&i](float x, float y)
	{
		return i.getPixel(x, y).r;
	});

	graph2.setRange(Range(0, 1, s.x-1), Range(0, 1, s.y-1));
	graph2.setPalette(Graph::Palette::Jet);
	
	Graph graph3;

	graph3.setEquation([](float x, float z)
	{
		Bolt bolt(210, 825, 890, 8, 12e-6);

		Layer alu(72, 400, 100, 20e-6);

		Layer gasket(1.5, 400, 2, 20e-6);

		Layer castIron(180, 400, 328, 10e-6);

		bolt.addLayer(alu);
		bolt.addLayer(gasket);
		bolt.addLayer(castIron);

		return bolt.getProperties(x, 10, z).leakageForceHot;

		/*float R = std::sqrt(x * x + z * z) + FLT_EPSILON;
		return std::sin(R) / R;*/
	});

	graph3.setRange(Range(0.0f, 0.1f, 1.0f), Range(10.0f, 1.f, 110.0f));
	graph3.setPalette(Graph::Palette::Cool);
	
	Graph graph4;

	graph4.setEquation([](float x, float z)
	{
		return std::cos(std::abs(x) + std::abs(z));
	});

	graph4.setRange(Range(-2.0f, 0.1f, 2.0f), Range(-2.0f, 0.1f, 2.0f));
	graph4.setPalette(Graph::Palette::Gray);

	m_graphs.push_back(std::move(graph1));
	m_graphs.push_back(std::move(graph2));
	m_graphs.push_back(std::move(graph3));
	//m_graphs.push_back(std::move(graph4));

	float pos = 0.f;

	for (auto &g : m_graphs)
	{
		g.setSize(Vector3f(100.f, 50.f, 100.f));
		g.setPosition(0.f, 0.f, pos);
		g.plot();
		pos += 100.f;
	}

	m_graphs[1].setSize(Vector3f(100.f, 10.f, 100.f));
	m_graphs[1].plot();

	Mesh mesh1("Stand.stl", Color::rgb(120, 120, 120));
	//
	Mesh mesh2("Stand2.stl", Color::rgb(150, 150, 150));

	m_meshes.push_back(std::move(mesh1));
	m_meshes.push_back(std::move(mesh2));

	Vector2f size(10, 12);

	mBlocks.reserve(200);

	for (int x = 0; x < size.x; ++x)
	{
		for (int y = 0; y < size.y; ++y)
		{
			Block b;
			b.create(0.5f);
			b.setTexture(context.textures->get(Textures::Texture1));
			b.setPosition(Vector3f(x, 0.0f, y));
			mBlocks.push_back(std::move(b));
		}
	}

	for (size_t i = 0; i < mBlocks.size(); ++i)
	{
		m_collidables.pushBack(&mBlocks[i]);
	}

	m_GBuffer.Init(context.window->getSize().x, context.window->getSize().y);

	m_pointLight.DiffuseIntensity = 0.2f;
	m_pointLight.AmbientIntensity = 0.8f;
	m_pointLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
	m_pointLight.Position = cam.getPosition();
	m_pointLight.Attenuation.Constant = 0.0f;
	m_pointLight.Attenuation.Linear = 0.0f;
	m_pointLight.Attenuation.Exp = 0.0008f;

	Graph2D graph2_1;
	graph2_1.setEquation([](float x)
	{
		if (x == 0)
		{
			return 1.f;
		}

		return std::sin(x) / x;
	});
	graph2_1.setRange(Range());
	graph2_1.setSize(Vector2f(100.f, 50.f));
	graph2_1.setPosition(Vector3f(200, 0, 0));
	graph2_1.plot();

	m_graphs2.push_back(std::move(graph2_1));

	//m_robot.plot();

	initializeButtons();
}

void GameState::draw()
{
	RenderTarget2D& target = *getContext().target;

	RenderStates states;
	states.shaderHolder = getContext().shaders;
	states.shader = &states.shaderHolder->get(Shaders::Default);

	m_GBuffer.BindForWriting();

	// Only the geometry pass updates the depth buffer
	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);

	//glEnable(GL_POLYGON_OFFSET_LINE);
	//
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glPolygonOffset(-1, -1);

	states.cam = &cam;

	Shader2D* s = &states.shaderHolder->get(Shaders::DeferredGeometry);

	s->setTextureUnit("gColorMap", 0);

	//s->setParameter("light.position", states.cam->getPosition());
	//s->setParameter("light.intensities", 1.f, 1.f, 1.f);
	states.shader = s;

	//for (auto & g : m_graphs)
	//{
	//	g.showWireframe(polygonMode);
	//	target.draw(g, states);
	//}

	//for (auto & g : m_graphs2)
	//{
	//	target.draw(g, states);
	//}

	//target.draw(m_robot, states);

	for (auto & m : m_meshes)
	{
		target.draw(m, states);
	}

	//for (auto &b : mBlocks)
	//{
	//	target.draw(b, states);
	//}


	glDepthMask(GL_FALSE);

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	m_GBuffer.BindForReading();
	glClear(GL_COLOR_BUFFER_BIT);

	Vector2i screenSize = getContext().window->getSize();

	s = &states.shaderHolder->get(Shaders::DeferredLighting);
	s->setTextureUnit("gPositionMap", GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	s->setTextureUnit("gColorMap", GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	s->setTextureUnit("gNormalMap", GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	s->setParameter("gScreenSize", screenSize);
	s->setParameter("gEyeWorldPos", cam.getPosition());
	s->setParameter("gMatSpecularIntensity", 0.4f);
	s->setParameter("gSpecularPower", 0.5f);

	m_pointLight.Position = cam.getPosition();

	setParameter(s, m_pointLight);

	states.shader = s;

	target.draw(m_quad, states);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//states.shader = &states.shaderHolder->get(Shaders::Default);

	//states.cam = &target.getCamera();

	//target.draw(mGUIContainer, states);
	//states.shader = &states.shaderHolder->get(Shaders::SDFF);
	//target.draw(mText, states);
}

bool GameState::update(Time dt)
{
	Window& window = *getContext().window;
	mGUIContainer.update();
	cam.Update(dt.asSeconds());

	return true;
}

bool GameState::handleEvent(const Event& event)
{
	if (!mGUIContainer.handleEvent(event))
	{
		cam.handleEvent(event);
	}
	
	if (event.type == Event::MouseButtonPressed && (event.mouseButton.button == Mouse::Right || event.mouseButton.button == Mouse::Middle))
	{
		Rayf ray = MousePicker::generateRay(*getContext().window, cam);

		Vector3f camPos = cam.getPosition();

		if (m_collidables.testCollisions(ray))
		{
				Block *b = static_cast<Block*>(m_collidables.getBestCollision());
			
				auto r = m_collidables.getBestCollisionResult();

				auto newPos = b->getPosition() + r.normal;

				if (event.mouseButton.button == Mouse::Right)
				{
					Block block;
					block.create(0.5f);
					block.setPosition(newPos);
					block.setTexture(getContext().textures->get(Textures::Texture1));

					float h = randomize(0.f, 1.f);
					float s = randomize(0.1f, 0.5f);
					float l = 0.9f;
					block.setColour(Color::HSV(h, 1.0f, 1.0f));
					std::cout << mBlocks.capacity() << std::endl;
					mBlocks.push_back(std::move(block));

					m_collidables.pushBack(&mBlocks.back());
				}
				//else
				//{
				//	auto position = std::find(mBlocks.begin(), mBlocks.end(), &b);
				//	if (position != mBlocks.end())
				//	{// == vector.end() means the element was not found
				//		mBlocks.erase(position);
				//		m_collidables.eraseCollided();
				//	}
				//}
		}
	}

	if (event.type == Event::Resized)
	{
		cam.init(getContext().window);
		m_GBuffer.Init(event.size.width, event.size.height);
		//camera.setViewSize(static_cast<Vector2f>(getContext().window->getSize()));
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
			Mesh mesh(event.fileDrop.files[0], Color::rgb(150, 150, 150), true);
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

	auto resolution = GUI::Slider<float>::create(getContext());
	resolution->setSize(Vector2f(250.0f, 45.0f));
	resolution->setPosition(Vector2f(20.0f, 20.0f));
	resolution->setPossibleValues(scaleSpeed);
	resolution->setCurrentValue(1.0f);
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
