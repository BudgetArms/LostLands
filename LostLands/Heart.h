#pragma once
class Heart
{
public:
	Heart(const Point2f& position, float width = 100.f);


	void Draw() const;

	void SetWidth(float width)
	{
		m_Width = width;
	}


	Point2f m_Position;


private:
	float m_Width;


};


