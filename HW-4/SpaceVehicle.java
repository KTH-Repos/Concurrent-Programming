public class SpaceVehicle extends Thread {

    int nitorgenFuelTankCapacity;
    int quantumFluidTankCapacity;
    int nitrogenFuelTank;
    int quantumFluidTank;
    SpaceStation station;

    boolean takesNitrogen;
    boolean takesQuantumFluid;

    public SpaceVehicle(String name, int nitorgenFuelTankCapacity, int quantumFluidTankCapacity, SpaceStation station) {
        super(name);
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
            station.getFuel(this);
            try {
                System.out.println("Vehicle " + this.getName() + " will now travel away ...");
                System.out.println("  ");
                sleep(5000);
                nitrogenFuelTank = 0;
                quantumFluidTank = 0;
            } catch (InterruptedException e) {
                System.out.println("Exception occured " + e);
            }
        }
    }

}
