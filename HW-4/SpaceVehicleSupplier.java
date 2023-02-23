public class SpaceVehicleSupplier extends Thread {
    int nitorgenFuelTankCapacity;
    int quantumFluidTankCapacity;
    int nitrogenFuelTank;
    int quantumFluidTank;

    int nitrogenFuelSupply;
    int quantumFluidSupply;
    boolean suppliesNitorgen;
    boolean suppliesQuantumFluid;

    SpaceStation station;
    boolean takesNitrogen;
    boolean takesQuantumFluid;

    public SpaceVehicleSupplier(int nitroFuelTankCapacity, int quantFluidTankCapacity, int nitroFuelSupply,
            int quantFluidSupply, SpaceStation station) {
        this.station = station;
        this.nitorgenFuelTankCapacity = nitroFuelTankCapacity;
        this.quantumFluidTankCapacity = quantFluidTankCapacity;
        this.nitrogenFuelSupply = nitroFuelSupply;
        this.quantumFluidSupply = quantFluidSupply;

        if (this.nitorgenFuelTankCapacity > 0) {
            this.takesNitrogen = true;
        }

        if (this.quantumFluidTankCapacity > 0) {
            this.takesQuantumFluid = true;
        }

        if (this.nitrogenFuelSupply > 0) {
            this.suppliesNitorgen = true;
        }
        if (this.quantumFluidSupply > 0) {
            this.suppliesQuantumFluid = true;
        }
    }

    @Override
    public void run() {
        while (true) {
            station.setFuel(this);
            try {
                System.out.println("Supplier" + this.getName() + " will now travel away ...");
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
