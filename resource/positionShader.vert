layout(location = 0) in vec4 __qyt_position;
layout(location = 1) in vec4 __qyt_color;

out vec4 color;

void main()
{

    gl_Position =  __qyt_mvpMatrix * __qyt_position;
//    gl_Position =  __qyt_position;
    color = __qyt_color;
}

