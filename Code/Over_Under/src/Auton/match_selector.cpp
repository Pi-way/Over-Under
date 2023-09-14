#include "vex.h"
using namespace vex;

FormatedText::FormatedText(char* _text, const char* _color, int width, int height) :
    text {_text},
    color {_color}
{
    this->Center(width, height);
}

void FormatedText::Draw(int x_offset, int y_offset)
{
    Brain.Screen.setPenColor(color);
    Brain.Screen.printAt(x + x_offset, y + y_offset, text);
}

void FormatedText::Center(int width, int height)
{
    //update x position
    x = (width - Brain.Screen.getStringWidth(text)) / 2 + 1;
    y = (height / 2) + 9;
}



Button::Button(int _x, int _y, int _width, int _height, char* _text) : 
    x {_x},
    y {_y},
    width {_width},
    height {_height},
    text {FormatedText(_text, "#ffffff", _width, _height)}
{
    DrawReleased();
}

void Button::DrawReleased()
{
    Brain.Screen.setFillColor(fill_color);
    Brain.Screen.setPenColor(outline_color_released);
    Brain.Screen.setPenWidth(4);

    Brain.Screen.drawRectangle(x, y, width, height);
    text.Center(width, height);
    text.Draw(x, y);
}

void Button::DrawPressed()
{
    Brain.Screen.setFillColor(fill_color);
    Brain.Screen.setPenColor(outline_color_pressed);
    Brain.Screen.setPenWidth(4);

    Brain.Screen.drawRectangle(x, y, width, height);
    text.Center(width, height);
    text.Draw(x, y);
}

void Button::Center()
{
    text.Center(width, height);
}

bool Button::Update()
{
    pressing_before = pressing_now;
    pressing_now = Brain.Screen.pressing();
    int press_x = (!pressing_now) ? 0 : Brain.Screen.xPosition();
    int press_y = (!pressing_now) ? 0 : Brain.Screen.yPosition();

    bool x_invalid = press_x < x || press_x > x + width;
    bool y_invalid = press_y < y || press_y > y + height;

    was_valid = is_valid;
    is_valid = !x_invalid && !y_invalid;

            
    if(!pressing_now && pressing_before || updates_to_go > 0){
        updates_to_go --;
        DrawReleased();
    }

    if(was_valid && !is_valid){
        if(pressing_before && !pressing_now){
            updates_to_go = 1;
            DrawReleased();
            return true;
        }
    }

    if(is_valid){
        if(pressing_now && !pressing_before){
            DrawPressed();
            return false;
        }
    }

    return false;
}



Alliance::Alliance(){}

void Alliance::Next()
{
    index = (index + 1) % 3;
}

char* Alliance::String()
{
    return strings[index];
}

const char* Alliance::Color()
{
    return colors[index];
}



Side::Side(){}

void Side::Next()
{
    index = (index + 1) % 3;
}

char* Side::String()
{
    return strings[index];
}



Type::Type(){}

void Type::Next()
{
    index = (index + 1) % 4;
}

char* Type::String()
{
    return strings[index];
}

NoAuto::NoAuto(){}

void NoAuto::Next()
{
    index = (index + 1) % 2;
}

char* NoAuto::String()
{
    return strings[index];
}



MatchSelector::MatchSelector()
{
    alliance_button.fill_color = "#dd0000";
    Brain.Screen.setFont(vex::fontType::prop20);

    alliance_button.DrawReleased();
    side_button.DrawReleased();
    type_button.DrawReleased();
    callibrate_button.DrawReleased();

    for(int a = 0; a < 2; a ++){
        for(int s = 0; s < 3; s++){
            for(int t = 0; t < 4; t++){
                autos[a][s][t] = {
                    [](){
                        Brain.Screen.clearScreen();
                        Brain.Screen.setFillColor("#000000");
                        Brain.Screen.setFont(vex::fontType::mono60);
                        Brain.Screen.printAt(80, 130, "EMPTY AUTO!");
                    },
                    "This auton is not initialized."
                };
            }
        }
    }

    ShowSelectionDescription();
}

void MatchSelector::Assign(const char* a, const char* s, const char* t, auton* autonomous)
{
    int al;
    int si;
    int ty;

    if		(a == "Match")	{al = 0;}
    else if	(a == "Skills")	{al = 1;}

    if		(s == "Left")	{si = 0;}
    else if	(s == "Center")	{si = 1;}
    else if (s == "Right")	{si = 2;}

    if		(t == "Simple")			{ty = 0;}
    else if	(t == "Complex")		{ty = 1;}
    else if (t == "Half Winpoint")	{ty = 2;}
    else if (t == "Winpoint")		{ty = 3;}

    autos[al][si][ty] = *autonomous;
}

std::pair<std::function<void()>, std::string> MatchSelector::CurrentSelection()
{
    if( alliance.String() == "Red" || alliance.String() == "Blue")
    {
        //Running matches
        return autos[0][side.index][type.index];
    }
    else
    {
        //Running skills
        return autos[1][side.index][type.index];
    }
}

void MatchSelector::Update()
{   
    if(!should_update){return;}

    bool a_pressed = alliance_button.Update();
    bool s_pressed = side_button.Update();
    bool t_pressed = type_button.Update();
    bool run_pressed = should_run_auto_button.Update();

    bool callibrate_pressed = callibrate_button.Update();
    if(callibrate_pressed)
    {   
        msTask = vex::task(Callibrate);
        StartCountdown();

        alliance_button.DrawReleased();
        side_button.DrawReleased();
        type_button.DrawReleased();
        should_run_auto_button.DrawReleased();
        ShowSelectionDescription();
    }

    selection_has_changed = a_pressed || s_pressed || t_pressed;

    if(a_pressed)
    {
        alliance.Next();
        alliance_button.text.text = alliance.String();
        alliance_button.fill_color = alliance.Color();
    }

    if(s_pressed)
    {
        side.Next();
        side_button.text.text = side.String();
    }

    if(t_pressed)
    {
        type.Next();
        type_button.text.text = type.String();
    }

    if(run_pressed)
    {
        no_auto.Next();
        should_run_auto_button.text.text = no_auto.String();
        run_nothing = !run_nothing;
        Controller.Screen.clearLine(3);
        Controller.Screen.setCursor(3,1);
        Controller.Screen.print(run_nothing);
    }

    alliance_button.Update();
    side_button.Update();
    type_button.Update();
    callibrate_button.Update();
    should_run_auto_button.Update();

    if(selection_has_changed)
    {
        ShowSelectionDescription();
    }
}

void MatchSelector::ShowSelectionDescription()
{   
    //Tokenize description:
    std::string description = CurrentSelection().second;
    std::vector<std::string> tokens;
    std::stringstream check1(description);
    std::string intermediate;

    while(getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }

    int left_stop = 170;
    int right_stop = 470;

    int line_height = 25;
    int space_width = 4;

    int line_start = 25;

    int index = 0;
    int current_line = 0;
    int current_x_pos = left_stop;
    int current_token_width = 0;

    Brain.Screen.setPenColor("#000000");
    Brain.Screen.setFillColor("#000000");
    Brain.Screen.drawRectangle(168, 0, 480-168, 143);

    Brain.Screen.setPenColor("#ffffff");

    while(index < tokens.size())
    {   
        current_token_width = Brain.Screen.getStringWidth(tokens[index].c_str());
        if (current_x_pos + current_token_width <= right_stop)
        {
            //okay to draw
            Brain.Screen.printAt(current_x_pos, line_start + current_line * line_height, tokens[index].c_str());
            current_x_pos += current_token_width + space_width;
        }
        else
        {
            //move to next line
            current_line ++;
            current_x_pos = left_stop;
            Brain.Screen.printAt(current_x_pos, line_start + current_line * line_height, tokens[index].c_str());
            current_x_pos += current_token_width + space_width;
        }
        index ++;
    }
}

AllianceEnum MatchSelector::GetAlliance()
{
    if(alliance.index == 0)
    {
        return Red;
    }
    if(alliance.index == 1)
    {
        return Blue;
    }
    return Skills;

}

void MatchSelector::StartCountdown()
{   
    Brain.Screen.clearScreen("#555555");
    Brain.Screen.setFillColor("#555555");
    Brain.Screen.setFont(vex::fontType::prop60);

    msTask = task([]()->int{Controller.rumble("-"); return 0;});
    Brain.Screen.printAt(100, 130, "3");
    wait(1, sec);
    msTask = task([]()->int{Controller.rumble("-"); return 0;});
    Brain.Screen.printAt(225, 130, "2");
    wait(1, sec);
    msTask = task([]()->int{Controller.rumble("-"); return 0;});
    Brain.Screen.printAt(350, 130, "1");
    wait(1, sec);
    msTask = task([]()->int{Controller.rumble("-"); return 0;});
    Brain.Screen.clearScreen("#000000");
    Brain.Screen.setFont(vex::fontType::prop20);
}

void MatchSelector::RunAutonomous()
{   
    should_update = false;



    //If the match selector is being used, and no auton is selected (or no auton should be ran)
    if (run_nothing && (Competition.isFieldControl())) {
        Brain.Screen.clearScreen();
        Brain.Screen.setFillColor("#000000");
        Brain.Screen.setFont(vex::fontType::mono60);
        Brain.Screen.printAt(80, 130, "EMPTY AUTO!");

        return;
    }


    //If the match selector is being used, and an auton was selected
    if (!run_nothing) {
        CurrentSelection().first();

        return;
    }


    //If neither above scenario was true, run the test auton
   autos[test_auton[0]][test_auton[1]][test_auton[2]].first();
}
void MatchSelector::SetTestAutonomous(const char* a, const char* s, const char* t)
{
    int al;
    int si;
    int ty;

    if		(a == "Match")	{al = 0;}
    else if	(a == "Skills")	{al = 1;}

    if		(s == "Left")	{si = 0;}
    else if	(s == "Center")	{si = 1;}
    else if (s == "Right")	{si = 2;}

    if		(t == "Simple")			{ty = 0;}
    else if	(t == "Complex")		{ty = 1;}
    else if (t == "Half Winpoint")	{ty = 2;}
    else if (t == "Winpoint")		{ty = 3;}

    test_auton.push_back(al);
    test_auton.push_back(si);
    test_auton.push_back(ty);

}
