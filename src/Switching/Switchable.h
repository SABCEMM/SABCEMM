#ifndef SWITCHABLE_H
#define SWITCHABLE_H


class Switchable
{
public:
    virtual ~Switchable(){};
    virtual void setChartistShare(double chartistShare) = 0;
    virtual double getChartistShare() = 0;
    virtual double getPredisposition() = 0;
    virtual double getalpha_p1() = 0;
    virtual double getalpha_p2() = 0;
    virtual double getFundamentalPrice() = 0;
};

#endif // SWITCHABLE_H
