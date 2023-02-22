public class FuelSpaceStation {

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

    class SpaceStation {
        int docks;
        int nitorgenFuel;
        int quantumFluid;

        public SpaceStation() {
            this.docks = 5;
        }

        synchronized void setNitrogenFuel(int amount) {
            this.nitorgenFuel = amount;
        }

        synchronized void setQuantumFluid(int amount) {
            this.quantumFluid = amount;
        }

    }

    class SpaceVehicle extends Thread {
        int nitorgenFuelTankCapacity;
        int quantumFluidTankCapacity;
        int nitrogenFuelTank;
        int quantumFluidTank;

        boolean takesNitrogen;
        boolean takesQuantumFluid;

        public SpaceVehicle(int nitorgenFuelTankCapacity, int quantumFluidTankCapacity) {
            this.nitorgenFuelTankCapacity = nitorgenFuelTankCapacity;
            this.quantumFluidTankCapacity = quantumFluidTankCapacity;
            if (this.nitorgenFuelTankCapacity > 0) {
                this.takesNitrogen = true;
            }
            if (this.quantumFluidTankCapacity > 0) {
                this.takesQuantumFluid = true;
            }
        }

        @Override
        public void run() {

        }

    }

}