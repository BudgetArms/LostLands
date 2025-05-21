#pragma once
#include "Character.h"


class Enemy final : public Character
{
public:
    Enemy(const Point2f& position);


private:

    void Draw() const override;
    void Update(float elapsedSec) override;

    Rectf& GetHitBox() override;
    virtual void Shoot() override;



protected:


};


