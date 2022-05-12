#ifndef YOUR_DIET_PROJECT_PARSER_CLASS_H
#define YOUR_DIET_PROJECT_PARSER_CLASS_H

class Information {
    std::string dish_name;
    double calories;
    double proteins;
    double fats;
    double carbohydrates;
public:
    bool title_find(std::string& begin);
    void get_name(std::string& begin);
    void calories_find(std::string& begin);
    std::string getNAME() {return dish_name;};
    double getCAL() {return calories;};
    double getPROT() {return proteins;};
    double getFAT() {return fats;};
    double getCARB() {return carbohydrates;};
};


#endif //YOUR_DIET_PROJECT_PARSER_CLASS_H
