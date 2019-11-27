
class Tile
{
    public:
    virtual ~Tile();
    bool getIfTileIsEnterable() const;
    private:
    bool mEnterable{true};
};