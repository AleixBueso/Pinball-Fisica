#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h" 
#include "ModulePlayer.h"
#include "SDL\include\SDL_timer.h"

#define BOUNCER_TIME 200;

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	PinballMap = App->textures->Load("pinball/bluetable_base.png");

	CreatePinball();
	// TODO: Homework - create a sensor

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->textures->Unload(PinballMap);
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(PinballMap, 0, 0);
	
	p2List_item<PhysBody*>* tmp_1 = shellders.getFirst();
	while (tmp_1 != NULL)
	{
		if (tmp_1->data->hit_timer > 0)
		{
			if (SDL_TICKS_PASSED(SDL_GetTicks(), tmp_1->data->hit_timer) == false)
			{
				int x, y;
				tmp_1->data->GetPosition( x, y);
				App->renderer->Blit(tmp_1->data->texture, x, y);
			}

			else
			{
				tmp_1->data->hit_timer = 0;
				App->player->score += 20;
			}
			
		}
		tmp_1 = tmp_1->next;
	}

	p2List_item<PhysBody*>* tmp_2 = triangles.getFirst();
	while (tmp_2 != NULL)
	{
		if (tmp_2->data->hit_timer > 0)
		{
			if (SDL_TICKS_PASSED(SDL_GetTicks(), tmp_2->data->hit_timer) == false)
			{
				int x, y;
				tmp_2->data->GetPosition(x, y);
				App->renderer->Blit(tmp_2->data->texture, x, y);
			}

			else
			{
				tmp_2->data->hit_timer = 0;
				App->player->score += 20;
			}
		
		}
	tmp_2 = tmp_2->next;
	}

	p2List_item<PhysBody*>* tmp_3 = pokemon.getFirst();
	while (tmp_3 != NULL)
	{
		if (tmp_3->data->hit_timer > 0)
		{
			if (SDL_TICKS_PASSED(SDL_GetTicks(), tmp_3->data->hit_timer) == false)
			{
				int x, y;
				tmp_3->data->GetPosition(x, y);
				App->renderer->Blit(tmp_3->data->texture, x, y);
			}

			else
			{
				tmp_3->data->hit_timer = 0;
				App->player->score += 100;
			}
			
		}
		tmp_3 = tmp_3->next;
	}

	if (PsyDuck->hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), PsyDuck->hit_timer) == false)
		{
			int x, y;
			PsyDuck->GetPosition(x, y);
			App->renderer->Blit(PsyDuck->texture, x, y);
		}

		else
		{
			PsyDuck->hit_timer = 0;
			App->player->score += 50;
		}
	}
	
	char title[50];
	sprintf_s(title, "Balls: %d Score: %06d", App->player->lives, App->player->score);
	App->window->SetTitle(title);

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 16, b2_dynamicBody));
		// TODO 8: Make sure to add yourself as collision callback to the circle you creates
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* body1, PhysBody* body2)
{
	p2List_item<PhysBody*>* tmp_1 = shellders.getFirst();
	while (tmp_1 != NULL)
	{
		if (tmp_1->data == body1)
		{
			tmp_1->data->hit_timer = SDL_GetTicks() + BOUNCER_TIME;
			//App->audio->PlayFx(bouncer1.fx);
			LOG("Collision!");
			App->player->score += 20;
			return;
		}
		tmp_1 = tmp_1->next;
	}

	p2List_item<PhysBody*>* tmp_2 = triangles.getFirst();
	while (tmp_2 != NULL)
	{
		if (tmp_2->data == body1)
		{
			tmp_2->data->hit_timer = SDL_GetTicks() + BOUNCER_TIME;
			//App->audio->PlayFx(bouncer1.fx);
			LOG("Collision!");
			App->player->score += 30;
			return;
		}

		tmp_2 = tmp_2->next;
	}

	if (PsyDuck == body1)
	{
		PsyDuck->hit_timer = SDL_GetTicks() + BOUNCER_TIME;
		//App->audio->PlayFx(side_bouncer1.fx);
		LOG("Collision!");
		App->player->score += 50;
		return;
	}

	p2List_item<PhysBody*>* tmp_3 = pokemon.getFirst();
	while (tmp_3 != NULL)
	{
		if (tmp_3->data == body1)
		{
			tmp_3->data->hit_timer = SDL_GetTicks() + BOUNCER_TIME;
			//App->audio->PlayFx(bouncer1.fx);
			LOG("Collision!");
			App->player->score += 100;
			return;
		}
		tmp_3 = tmp_3->next;
	}
		return;
	}

void ModuleSceneIntro::CreatePinball()
{

	int pinball_body[80] = {
		640, 1100,
		641, 265,
		614, 194,
		574, 133,
		521, 80,
		449, 36,
		373, 18,
		262, 18,
		174, 44,
		110, 92,
		58, 158,
		34, 232,
		25, 308,
		21, 392,
		31, 468,
		55, 537,
		86, 589,
		86, 701,
		47, 720,
		29, 745,
		29, 888,
		204, 997,
		197, 1102,
		393, 1102,
		384, 994,
		552, 888,
		558, 742,
		538, 709,
		500, 696,
		498, 582,
		530, 537,
		554, 470,
		562, 394,
		562, 312,
		553, 234,
		529, 165,
		562, 208,
		581, 262,
		586, 313,
		582, 1100
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, pinball_body, 80));

	int upper_right[44] = {
		412, 115,
		458, 156,
		481, 199,
		495, 262,
		500, 329,
		500, 407,
		489, 475,
		459, 547,
		441, 550,
		407, 537,
		408, 508,
		421, 469,
		411, 447,
		315, 401,
		419, 401,
		417, 347,
		321, 348,
		416, 300,
		435, 280,
		437, 245,
		427, 218,
		411, 191
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, upper_right, 44));

	int  upper_left[50] = {
		171, 113,
		134, 146,
		112, 177,
		96, 224,
		88, 264,
		81, 303,
		82, 410,
		93, 470,
		107, 511,
		129, 547,
		140, 553,
		176, 538,
		176, 516,
		164, 481,
		168, 457,
		187, 445,
		263, 401,
		174, 399,
		175, 349,
		262, 346,
		165, 299,
		146, 278,
		145, 250,
		154, 221,
		171, 193
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, upper_left, 50));

	int left_triangle[6] = {
		178, 850,
		133, 750,
		128, 815
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, left_triangle, 6));

	int right_triangle[6] = {
		403, 851,
		453, 750,
		451, 819
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, right_triangle, 6));

	int right_thing[12] = {
		405, 929,
		392, 904,
		496, 835,
		498, 743,
		508, 743,
		509, 865
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, right_thing, 12));

	int left_thing[12] = {
		172, 927,
		188, 904,
		92, 837,
		85, 747,
		74, 747,
		75, 863
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, left_thing, 12));

	int bar1[8] = {
		238, 87,
		257, 87,
		259, 148,
		239, 148
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, bar1, 8));

	int bar2[8] = {
		327, 88,
		344, 88,
		344, 144,
		325, 144
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, bar2, 8));

	//Cloyster & Slowpoke
	pokemon.add(App->physics->CreateRectangle(160, 373, 36, 55, 1.2f, b2_staticBody))->data->listener = this;
	pokemon.getLast()->data->texture = App->textures->Load(""); //ASIER
	pokemon.add(App->physics->CreateRectangle(430, 373, 40, 57, 1.2f, b2_staticBody))->data->listener = this;
	pokemon.getLast()->data->texture = App->textures->Load(""); //ASIER

	//PsyDuck
	PsyDuck = App->physics->CreateCircle(498, 644, 65, b2_staticBody, 1.0f);
	PsyDuck->texture = App->textures->Load(""); //ASIER

	int boucing_rectangle_1[8] = {
		134, 761,
		142, 759,
		183, 836,
		172, 837
	};
	triangles.add(App->physics->CreatePinballChain(0, 0, boucing_rectangle_1, 8, 1.0f))->data->listener=this;
	triangles.getLast()->data->texture = App->textures->Load(""); //ASIER
	

	int boucing_rectangle_2[8] = {
		446, 763,
		436, 760,
		401, 830,
		409, 832
	};
	triangles.add(App->physics->CreatePinballChain(0, 0, boucing_rectangle_2, 8, 1.0f))->data->listener = this;
	triangles.getLast()->data->texture = App->textures->Load(""); //ASIER

	//Shellders
	shellders.add(App->physics->CreateCircle(215, 255, 40, b2_staticBody, 1.0f))->data->listener = this;
	shellders.getLast()->data->texture = App->textures->Load(""); //ASIER
	shellders.add(App->physics->CreateCircle(291, 191, 40, b2_staticBody, 1.0f))->data->listener = this;
	shellders.getLast()->data->texture = App->textures->Load(""); //ASIER
	shellders.add(App->physics->CreateCircle(368, 255, 40, b2_staticBody, 1.0f))->data->listener = this;
	shellders.getLast()->data->texture = App->textures->Load(""); //ASIER



	pinball.add(App->physics->CreateCircle(292, 990, 2, b2_staticBody, 1.0f));
}

// TODO 8: Now just define collision callback for the circle and play bonus_fx audio
