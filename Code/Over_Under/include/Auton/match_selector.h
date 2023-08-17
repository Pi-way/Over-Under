#pragma once

class FormatedText {
public:
	char* text;
	const char* color;
	int x;
	int y;

	FormatedText(char* _text, const char* _color, int width, int height);
	void Draw(int x_offset, int y_offset);
	void Center(int width, int height);
};

class Button{
public:

	FormatedText text;
	const char* fill_color = "#666666";
	const char* outline_color_released = "#ffff00";
	const char* outline_color_pressed = "#6699ff";

	int width;
	int height;
	int x;
	int y;

	bool pressing_now = false;
	bool pressing_before = false;

	bool was_valid = false;
	bool is_valid = false;

	int updates_to_go = 0;

	Button(int _x, int _y, int _width, int _height, char* _text);
	void DrawReleased();
	void DrawPressed();
	void Center();
	bool Update();

};

class Alliance {
public:
	int index = 0;
	char* strings[3] {
		"Red",
		"Blue",
		"Skills"
	};
	const char* colors[3] {
		"#dd0000",
		"#333399",
		"#aa0088"
	};
	Alliance();
	void Next();
	char* String();
	const char* Color();
};

class Side {
public:
	int index = 0;
	char* strings[3] {
		"Left",
		"Center",
		"Right"
	};
	Side();
	void Next();
	char* String();
};

class Type {
public:
	int index = 0;
	char* strings[4] {
		"Simple",
		"Complex",
		"Half Winpoint",
		"Winpoint"
	};
	Type();
	void Next();
	char* String();
};

class NoAuto {
public:
	int index = 0;
	char* strings[2] {
		"Don't Run",
		"Run Auto"
	};
	NoAuto();
	void Next();
	char* String();
};

typedef std::pair<std::function<void()>, const char*> auton;

enum AllianceEnum{
	Red,
	Blue,
	Skills
};

class MatchSelector {
public:

	Alliance alliance;
	Side side;
	Type type;
	NoAuto no_auto;

	Button alliance_button = Button(5, 5, 150, 71, "Red");
	Button side_button = Button(5, 84, 150, 71, "Left");
	Button type_button = Button(5, 163, 150, 71, "Simple");
	Button callibrate_button = Button(321, 163, 150, 71, "Callibrate");
	Button should_run_auto_button = Button(163, 163, 150, 71, "Don't Run");

	std::pair<std::function<void()>, std::string> autos[2][3][4];
	std::function<void()> *test_auton_ptr;

	bool selection_has_changed = false;
	bool should_update = true;
	bool run_nothing = true;

	MatchSelector();
	void Assign(const char* a, const char* s, const char* t, auton* autonomous);
	std::pair<std::function<void()>, std::string> CurrentSelection();
	void Update();
	void ShowSelectionDescription();
	AllianceEnum GetAlliance();
	void StartCountdown();
	void RunAutonomous();
	void SetTestAutonomous(const char* a, const char* s, const char* t);

};

extern MatchSelector ms;