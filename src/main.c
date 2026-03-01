#include <raylib.h>
#include <math.h>
#include <stdlib.h>

#define WIDTH 900
#define HEIGHT 600
#define SCALE 150

// --- SDF ---
float sdf_circle(float x, float y, float cx, float cy, float r)
{
	float dx = x - cx;
	float dy = y - cy;
	return sqrtf(dx * dx + dy * dy) - r;
}

float sigmoid(float t)
{
	return 1.0f / (1.0f + expf(-t));
}

// --- Color mapping ---
// Takes an SDF value, returns a Color
// For now: black if outside, white if inside
Color distance_to_color(float d)
{
	unsigned char v = (unsigned char)(255 * sigmoid(d / SCALE));
	return (Color){v, v, v, 255};
}

int main(void)
{
	InitWindow(WIDTH, HEIGHT, "SDF");
	SetTargetFPS(60);

	// Allocate pixel buffer: WIDTH * HEIGHT Color values
	Color *buffer = malloc(WIDTH * HEIGHT * sizeof(Color));

	// Create a raylib Image from the buffer, then a Texture2D to draw it
	Image img = {
		.data = buffer,
		.width = WIDTH,
		.height = HEIGHT,
		.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
		.mipmaps = 1};
	Texture2D tex = LoadTextureFromImage(img);

	while (!WindowShouldClose())
	{
		// Loop over every pixel
		for (int py = 0; py < HEIGHT; py++)
		{
			for (int px = 0; px < WIDTH; px++)
			{
				// Convert to math coords
				float x = px - WIDTH / 2;
				float y = HEIGHT / 2 - py;

				float d = sdf_circle(x, y, 0, 0, 150);
				buffer[py * WIDTH + px] = distance_to_color(d);
			}
		}

		UpdateTexture(tex, buffer);

		BeginDrawing();
		DrawTexture(tex, 0, 0, WHITE);
		EndDrawing();
	}

	UnloadTexture(tex);
	free(buffer);
	CloseWindow();
	return 0;
}