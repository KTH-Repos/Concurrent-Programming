import java.util.Random;

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
    Random random;
    boolean takesNitrogen;
    boolean takesQuantumFluid;

    public SpaceVehicleSupplier(String name, int nitroFuelTankCapacity, int quantFluidTankCapacity, int nitroFuelSupply,
            int quantFluidSupply, SpaceStation station) {
        super(name);
        this.station = station;
        this.random = new Random();
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
        int i = 0;
        int min = 1;
        int max = 10;
        while (i < 4) {
            station.setFuel(this);
            try {
                int sleepDuration = random.nextInt(max - min) + min;
                sleep(sleepDuration);
                nitrogenFuelTank = 0;
                quantumFluidTank = 0;
            } catch (InterruptedException e) {
                System.out.println("Exception occured " + e);
            }
            i++;
        }

    }
}
