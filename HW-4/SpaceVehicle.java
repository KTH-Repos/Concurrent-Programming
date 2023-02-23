public class SpaceVehicle extends Thread {

    int nitorgenFuelTankCapacity;
    int quantumFluidTankCapacity;
    int nitrogenFuelTank;
    int quantumFluidTank;
    SpaceStation station;

    boolean takesNitrogen;
    boolean takesQuantumFluid;

    public SpaceVehicle(int nitorgenFuelTankCapacity, int quantumFluidTankCapacity, SpaceStation station) {
        this.station = station;
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
        while (true) {
            if (takesNitrogen && takesQuantumFluid) {
                nitrogenFuelTank = station.getNitrogenFuel(nitorgenFuelTankCapacity);
                quantumFluidTank = station.getQuantumFluid(quantumFluidTankCapacity);
            } else if (takesNitrogen) {
                nitrogenFuelTank = station.getNitrogenFuel(nitorgenFuelTankCapacity);
            } else {
                quantumFluidTank = station.getQuantumFluid(quantumFluidTankCapacity);
            }

            try {
                sleep(5000);
                nitrogenFuelTank = 0;
                quantumFluidTank = 0;
            } catch (InterruptedException e) {
                System.out.println("Exception occured " + e);
            }
        }
    }

}
