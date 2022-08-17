#include "raylib.h"

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

	Ball ball = Ball(GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f);
	Paddle paddleLeft = Paddle(50, GetScreenHeight() * 0.5f);
	Paddle paddleRight = Paddle(GetScreenWidth() - 50.0f, GetScreenHeight() * 0.5f);

	const char* winnerText = nullptr;

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

		if (ball.position.x < 0)
		{
			winnerText = "Right Player Wins!";
		}
		if (ball.position.x > GetScreenWidth())
		{
			winnerText = "Left Player Wins!";
		}
		if (winnerText != nullptr && IsKeyPressed(KEY_SPACE))
		{
			ball.position.x = GetScreenWidth() * 0.5f;
			ball.position.y = GetScreenHeight() * 0.5f;
			ball.speed.x = 200.0f;
			ball.speed.y = 300.0f;
			winnerText = nullptr;
		}

		/********** Rendering *************/
		BeginDrawing();
		ClearBackground(BLACK);

		ball.Draw();
		paddleLeft.Draw();
		paddleRight.Draw();

		if (winnerText != nullptr)
		{
			DrawText(winnerText, GetScreenWidth() * 0.5f - MeasureText(winnerText, 60) * 0.5f, GetScreenHeight() * 0.5f, 60, YELLOW);
		}

		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}