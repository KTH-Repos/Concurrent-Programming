// a space station that supplies nitrogen and quantum fluid
// space station can handle V space vehicles, N liters of nitorgen & Q liters of
// quantum fluid

// a space vehicle arrives to station, requests the needed amount of the two
// different types of fuels or only one of them.
// If enough fuel isnt availabe of either type, the vehicle waits without
// blocking

// Special vehicles deliver fuel in fixed quantities that exceeds their tank
// capacity.
// When a special vehicle arrives, it delays until there is enough space to
// deposit the fuel delivered.

public class SpaceStation {
    int docks;
    int nitrogenFuel;
    int quantumFluid;

    public SpaceStation() {
        docks = 5;
    }

    synchronized void setNitrogenFuel(int amount) {
        while (amount > nitrogenFuel) {
            try {
                wait();
            } catch (InterruptedException e) {
                System.out.println("Exception occured " + e);
            }
        }
        nitrogenFuel += amount;
        notify();
    }

    synchronized void setQuantumFluid(int amount) {
        while (amount > quantumFluid) {
            try {
                wait();
            } catch (InterruptedException e) {
                System.out.println("Exception occured " + e);
            }
        }
        quantumFluid += amount;
        notify();
    }

    synchronized int getNitrogenFuel(int fuelRequired) {
        while (fuelRequired > nitrogenFuel) {
            try {
                wait();
            } catch (InterruptedException e) {
                System.out.println("Exception occured " + e);
            }
        }
        nitrogenFuel -= fuelRequired;
        notify();
        return fuelRequired;
    }

    synchronized int getQuantumFluid(int fuelRequired) {
        while (fuelRequired > quantumFluid) {
            try {
                wait();
            } catch (InterruptedException e) {
                System.out.println("Exception occured " + e);
            }
        }
        quantumFluid -= fuelRequired;
        notify();
        return fuelRequired;
    }

}
