#include "ShareCalculator.h"
#include <cmath>

double ShareCalculator::ratePrices(double price, double fundamentalPrice)
{
    const double ratio = price/fundamentalPrice;

    switch (strategy) {
        // Variante 1: langzeitstabil
        // ratio ist immer < 1 (???!!??)
        // ganz merkwürdige Autokorrelation
        // komischer QQ
        case pdivpf:
            return pow((price-fundamentalPrice)/price, 2);

        // Variante 3:
        // eigentlich gar nicht so schlecht - stabil bis mind. 5000 Schritte
        // allgemein recht kleiner chartist share, leider. 0.3 ist Maximum
        // QQ ist ok (rechts extremer als links)
        case ShareCalculatorStrategy::ratio:
            return ratio;

        // Variante 2 (bezieht sich auf meine eigene Funktion
        // Maximum 0.8, sehr spikey - ein bisschen so wie DCA-W bei FW
        // ratio ist manchmal mehr und manchmal weniger
        // QQ ist ok, aber der share ist absoluter Blödsinn
        // ist die Berechnung Blödsinn? Ich möchte ja Preise nahe p_f ja nicht penalisieren (ich will dort also 0 zurückgeben!)
        // TODO: ja, genau so - die Formel unten ist also falsch.
        case ratiowithfunction:
            if(ratio >= 5 || ratio <= 0.2)
                return 0;
            if(ratio <= 1)
                return 5*ratio/4. - 1/5.;
            else
                return -ratio/4.+5/4.;

        default:
            throw "invalid share calculator strategy.";
    }
}

void ShareCalculator::updateShares()
{
    for(std::size_t i = 0; i < switchables.size(); ++i)
    {
        Switchable* switchable = switchables[i];

        // attractiveness is not yet updated, and therefore a_{t-1} (as intended)
        double n_c_new = 1. / (1 + exp(beta * attractiveness.at(i)));

        switchable->setChartistShare(n_c_new);

        double n_c = switchable->getChartistShare();
        double n_f = 1-n_c;

        // calculate index of relative attractiveness
        // currently, only with DCA-HPM.
        attractiveness.at(i) = alpha_n*(n_f-n_c)+
                alpha_0+
                switchable->getalpha_p1()*pow(price.getPrice() - switchable->getFundamentalPrice(), 2)+
                switchable->getalpha_p2()*ratePrices(price.getPrice(), switchable->getFundamentalPrice());
    }
}

const std::map<std::string, ShareCalculator::ShareCalculatorStrategy> ShareCalculator::stringToShareCalculatorStrategy =
        boost::assign::map_list_of("pdivpf", ShareCalculatorStrategy::pdivpf)("ratio", ShareCalculatorStrategy::ratio)
                ("ratiowithfunction", ShareCalculatorStrategy::ratiowithfunction);

ShareCalculator::ShareCalculator(std::vector<Switchable*> switchables, Price& price, std::string strategy, double alpha_n, double alpha_0):
alpha_n(alpha_n), alpha_0(alpha_0), switchables(switchables), price(price){
    // choose initial attractiveness such that initial chartist share is maintained
    this->strategy = stringToShareCalculatorStrategy.at(strategy);
    for(std::size_t i = 0; i < switchables.size(); ++i)
        attractiveness.push_back(log(1/(switchables.at(i)->getChartistShare())-1)/beta);
}