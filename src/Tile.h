
class Tile
{
    public:
    virtual ~Tile()=default;
    bool getIfTileIsEnterable() const;
    private:
    bool mEnterable{true};
};