#include "raylib.h"
#include <string>
#include <iostream>

struct object
{
	Vector2 position;
	virtual void Update() {}
	virtual void Draw() {}
};

class Ball : public object
{
public:
	Vector2 speed;
	float radius;

public:
	Ball(float _x, float _y)
	{
		position.x = _x;
		position.y = _y;
		radius = 5;
		speed.x = 200.0f;
		speed.y = 300.0f;
	}

	void Update() override
	{
		position.x += speed.x * GetFrameTime();
		position.y += speed.y * GetFrameTime();

		if (position.y < 0)
		{
			position.y = 0;
			speed.y *= -1.0f;
		}
		if (position.y > GetScreenHeight())
		{
			position.y = GetScreenHeight();
			speed.y *= -1.0f;
		}
	}

	void Draw() override
	{
		DrawCircle(position.x, position.y, radius, WHITE);
	}
};

class Paddle : public object
{
public:
	float width, height;
	float speed;
public:
	Paddle(float _x, float _y)
	{
		position.x = _x;
		position.y = _y;
		width = 10.0f;
		height = 100.0f;
		speed = 500.0f;
	}

	Rectangle GetRectangle()
	{
		return Rectangle{ position.x - width * 0.5f, position.y - height * 0.5f, width, height };
	}

	void Draw() override
	{
		DrawRectangleRec(GetRectangle(), WHITE);
	}

	void Update(bool _isMovingUp)
	{
		if (_isMovingUp == true)
		{
			if (position.y < height * 0.5f)
			{
				position.y = height * 0.5f;
				return;
			}
			position.y -= speed * GetFrameTime();
		}
		else
		{
			if (position.y > GetScreenHeight() - height * 0.5f)
			{
				position.y = GetScreenHeight() - height * 0.5f;
				return;
			}
			position.y += speed * GetFrameTime();
		}

	}

};

int main()
{
	InitWindow(800, 600, "Pong");
	SetWindowState(FLAG_VSYNC_HINT);

	int leftPoints = 0, rightPoints = 0;

	Ball ball = Ball(GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f);
	Paddle paddleLeft = Paddle(50, GetScreenHeight() * 0.5f);
	Paddle paddleRight = Paddle(GetScreenWidth() - 50.0f, GetScreenHeight() * 0.5f);

	std::string winnerText;
	std::string leftPointText = std::to_string(leftPoints);
	std::string rightPointText = std::to_string(rightPoints);
	bool hasRoundPaused = false;


	while (WindowShouldClose() == false)
	{

		/********** Update *************/
		ball.Update();

		if (IsKeyDown(KEY_W))
		{
			paddleLeft.Update(true);
		}
		else if (IsKeyDown(KEY_S))
		{
			paddleLeft.Update(false);
		}

		if (IsKeyDown(KEY_UP))
		{
			paddleRight.Update(true);
		}
		else if (IsKeyDown(KEY_DOWN))
		{
			paddleRight.Update(false);
		}

		if (CheckCollisionCircleRec(ball.position, ball.radius, paddleLeft.GetRectangle()))
		{
			if (ball.speed.x < 0)
			{
				ball.speed.x *= -1.1f;
				ball.speed.y = (ball.position.y - paddleLeft.position.y) / (paddleLeft.height * 0.5f) * ball.speed.x;
			}
		}
		if (CheckCollisionCircleRec(ball.position, ball.radius, paddleRight.GetRectangle()))
		{
			if (ball.speed.x > 0)
			{
				ball.speed.x *= -1.1f;
				ball.speed.y = (ball.position.y - paddleRight.position.y) / (paddleRight.height * 0.5f) * -ball.speed.x;
			}
		}


		if (hasRoundPaused == false)
		{
			if (ball.position.x < 0)
			{
				if (rightPoints <= 5)
				{
					winnerText = "Right Player Scores!\nSpace To Continue";
					++rightPoints;
					rightPointText = std::to_string(rightPoints);
				}
				else
				{
					winnerText = "Right Player Wins!\nSpace To Restart";
				}
				hasRoundPaused = true;
			}
			if (ball.position.x > GetScreenWidth())
			{
				if (leftPoints <= 5)
				{
					winnerText = "Left Player Scores!\nSpace To Continue";
					leftPoints++;
					leftPointText = std::to_string(leftPoints);
				}
				else
				{
					winnerText = "Left Player Wins!\nSpace To Restart";
				}
				hasRoundPaused = true;
			}
		}
		else
		{
				
			if (winnerText != "" && IsKeyPressed(KEY_SPACE))
			{
				winnerText.clear();
				hasRoundPaused = false;

				ball.position.x = GetScreenWidth() * 0.5f;
				ball.position.y = GetScreenHeight() * 0.5f;
				ball.speed.x = 200.0f;
				ball.speed.y = 300.0f;
			}
		}

		/********** Rendering *************/
		BeginDrawing();
		ClearBackground(BLACK);

		ball.Draw();
		paddleLeft.Draw();
		paddleRight.Draw();

		if (winnerText != "")
		{
			DrawText(winnerText.c_str(),
				GetScreenWidth() * 0.5f - MeasureText(winnerText.c_str(), 60) * 0.5f,
				GetScreenHeight() * 0.5f - 50,
				60, YELLOW);
		}
		DrawText(leftPointText.c_str(), (GetScreenWidth() * 0.5f) - 200, 50, 45, WHITE);
		DrawText(rightPointText.c_str(), (GetScreenWidth() * 0.5f) + 200, 50, 45, WHITE);

		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}