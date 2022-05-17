#include <iostream>
#include "Coordinator.h"
#include <random>
#include <vector>
#include <chrono>

//demo 
Coordinator gCoordinator;
struct vec3
{
	union {
		struct { float a, b, c; };
		float mArry[3];
	};

	vec3& operator=(vec3& other)
	{
		a = other.a;
		b = other.b;
		c = other.c;
		return(*this);
	}

	vec3 operator*(float s) const
	{
		return { a * s, b * s, c * s };
	}
	vec3& operator+=(vec3& o)
	{
		a += o.a;
		b += o.b;
		c += o.c;
		return (*this);
	}

	vec3& operator+=(const vec3& o)
	{
		a += o.a;
		b += o.b;
		c += o.c;
		return (*this);
	}

	vec3& operator/=(vec3& o)
	{
		a /= o.a;
		b /= o.b;
		c /= o.c;
		return (*this);
	}
	vec3 operator+(vec3& other)
	{
		return { a + other.a, b + other.b, c + other.c};
	}
	vec3 operator/(vec3& o)
	{
		return { a / o.a, b / o.b, c / o.c };
	}
};

struct Gravity
{
	vec3 force;
};

struct RigidBody
{
	vec3 velocity;
	vec3 acceleration;
};

struct Transform
{
	vec3 position;
	vec3 rotation;
	vec3 scale;
};

class PhysSys : public System
{
public:
	void Update(float dt)
	{
		for (auto& entt : mEntities)
		{
			auto& rigidBody = gCoordinator.GetComponent<RigidBody>(entt);
			auto& transform = gCoordinator.GetComponent<Transform>(entt);
			auto& gravity = gCoordinator.GetComponent<Gravity>(entt);

			transform.position += rigidBody.velocity * dt;
			rigidBody.velocity += gravity.force * dt;

		}
	}

	virtual ~PhysSys() = default;

};

int main(int argc, char** argv)
{
	gCoordinator.Init();
	gCoordinator.RegisterComponent<Gravity>();
	gCoordinator.RegisterComponent<Transform>();
	gCoordinator.RegisterComponent<RigidBody>();

	auto physys = gCoordinator.RegisterSystem<PhysSys>();
	signature_t signature;
	gCoordinator.SetUpSignature<Transform, Gravity, RigidBody>(signature);
	gCoordinator.SetSystemSignature<PhysSys>(signature);

	//create the entities
	std::vector<entity_t> Entities(MAX_ENTITIES);
	std::default_random_engine engine;
	using u_r_d = std::uniform_real_distribution<float>;
	u_r_d randPos(-100.0f, 100.0f);
	u_r_d randRotation(0.0f, 3.0f);
	u_r_d randScale(3.0f, 5.0f);
	u_r_d randGravity(-10.0f, -1.0f);

	float scale = randScale(engine);
	for (auto& entt : Entities ) {
		entt = gCoordinator.CreateEntity();
		gCoordinator.AddComponent(entt, RigidBody{ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
		gCoordinator.AddComponent(entt, Gravity{ 0.0f, randGravity(engine), 0.0f });
		gCoordinator.AddComponent(entt, Transform{ {randPos(engine), randPos(engine), randPos(engine)}, 
			{randRotation(engine), randRotation(engine), randRotation(engine)}, {scale, scale, scale}});
	}
	float dt = 0.0f;
	std::string quit = "y";
	while (quit == "y")
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		physys->Update(dt);
		auto stoptime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(stoptime - startTime).count();
		std::cout << "Ran for: " << dt << " seconds, Run again(Y/N)";
		std::cin >> quit;

	}

	std::cout << "NECS test" << std::endl;
	return 0;
}