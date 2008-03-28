#include ".\barracks.h"
#include "ui.h"

extern UI *ui;

Barracks::Barracks(void)
{
}

Barracks::~Barracks(void)
{
}

void Barracks::init() {
	Building::init();

	size = Vector(3,2,3);
	scale = 0.2f;

	SetModel("data/models/barracks.md2","data/models/barracks.JPG");
	toolWindow = (UIWindow *)ui->CreateFromFile("data/UI/barracks.ui");

	tooltip.enabled = true;
	tooltip.tooltip = "Barracks";
}

void Barracks::Think() {
	spawnPoint = position + Vector(4,0,0);
}