//
//  items.hpp
//  text-adventure
//
//  Created by Veronica Hage on 2020-12-13.
//  Copyright © 2020 Veronica Hage. All rights reserved.
//

#pragma once

#ifndef items_hpp
#define items_hpp

#include <stdio.h>
#include <string>

#endif /* items_hpp */

class BaseItem {
public:
    BaseItem(const int id);
    virtual void useItem() = 0;
    
    int id;
    std::string name;
    std::string keySymbol;
};

class TeleportScroll : public BaseItem {
public:
    TeleportScroll(const int itemId, const std::string& locationId);
    
    virtual void useItem();
    
private:
    std::string teleportLocationId;
};
