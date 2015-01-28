#ifndef MONSTER_H_INCLUDED
#define MONSTER_H_INCLUDED
class Monster::public unit{
    public:
        enum Type{
            Eyeball

        };

    public:
        explicit    Monster(Type type);
    private:
        Type        mType;
};

Monster::Monster(Type type):mType(type){

}
#endif // MONSTER_H_INCLUDED
