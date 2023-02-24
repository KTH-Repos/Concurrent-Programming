import java.util.Random;

public class SpaceVehicle extends Thread {

    int nitorgenFuelTankCapacity;
    int quantumFluidTankCapacity;
    int nitrogenFuelTank;
    int quantumFluidTank;
    SpaceStation station;
    Random random;

    boolean takesNitrogen;
    boolean takesQuantumFluid;

    public SpaceVehicle(String name, int nitorgenFuelTankCapacity, int quantumFluidTankCapacity, SpaceStation station) {
        super(name);
        this.station = station;
        this.random = new Random();
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
        int i = 0;
        int min = 1;
        int max = 10;
        while (i < 4) {
            station.getFuel(this);
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
