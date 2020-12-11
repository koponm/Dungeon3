#ifndef HUD_H_
#define HUD_H_
#pragma once
#include "renderable.h"
#include "items.h"
#include "player.h"

#include <memory>

class HUD_object :
    public Renderable
{
public:
    HUD_object(const Texture& texture, int x, int y, ItemType type, int frame);
    ~HUD_object() {}

    void UpdateHUD(std::shared_ptr<Player> player);

private:
    int x_ = 0;
    int y_ = 0;
    ItemType type_;
};

#endif // HUD_H_