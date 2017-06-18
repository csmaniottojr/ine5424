#include <smart_data.h>
#include <utility/ostream.h>
#include <alarm.h>

using namespace EPOS;

const TSTP::Time DATA_PERIOD = 10 * 1000000;
const TSTP::Time DATA_EXPIRY = 2 * DATA_PERIOD;
const TSTP::Time INTEREST_EXPIRY = 5 * 60 * 1000000;

int main()
{
    // Interest center points
    TSTP::Coordinates center(0, 600, 100);

    // Regions of interest
    TSTP::Time start = TSTP::now();
    TSTP::Time end = start + INTEREST_EXPIRY;
    TSTP::Region region(center, 800, start, end);

    // Data of interest
    Switch data(region, DATA_EXPIRY, DATA_PERIOD);

    OStream cout;

    while(TSTP::now() < end) {
        Alarm::delay(DATA_PERIOD);

        Switch::Value state = data;
        TSTP::Time at = data.time();

        cout << "Now = " << TSTP::now() << endl;
        cout << "State at " << at << " is " << (state ? "on" : "off") << endl;

        data = !state;
    }

    cout << "Done!" << endl;

    return 0;
}
