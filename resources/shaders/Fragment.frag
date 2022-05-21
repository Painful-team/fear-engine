#version 460
out vec4 FragColor;
in float frame;

void main()
{
	if(frame == 0)
	{
		FragColor = vec4(0.50196078431f, 0.50196078431f, 0.50196078431f, 1.0f);
	}
	else
	{
		FragColor = vec4(0, 0, 0, 1.0f);
	}
}
