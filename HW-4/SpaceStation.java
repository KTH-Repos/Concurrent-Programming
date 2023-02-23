import java.util.Random;

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

    public SpaceStation(int docks, int nitrogenFuel, int quantumFluid) {
        this.docks = docks;
        this.nitrogenFuel = nitrogenFuel;
        this.quantumFluid = quantumFluid;
    }

    synchronized void setFuel(SpaceVehicleSupplier vehicle) {
        if (vehicle.suppliesNitorgen && vehicle.suppliesQuantumFluid) {
            while (vehicle.nitrogenFuelSupply > nitrogenFuel || vehicle.quantumFluidSupply > quantumFluid) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    System.out.println("Exception occured " + e);
                }
            }
            nitrogenFuel += vehicle.nitrogenFuelSupply;
            quantumFluid += vehicle.quantumFluidSupply;
            System.out.println("Station filled with " + vehicle.nitrogenFuelSupply + " of nitro & "
                    + vehicle.quantumFluidSupply + " of quantum by supplier " + vehicle.getName());
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
            getFuelForSupplier(vehicle);
            notifyAll();
        } else if (vehicle.suppliesQuantumFluid) {
            while (vehicle.quantumFluidSupply > quantumFluid) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    System.out.println("Exception occured " + e);
                }
            }
            quantumFluid += vehicle.quantumFluidSupply;
            System.out.println("Station filled with " + vehicle.quantumFluidSupply + " of quantum by supplier "
                    + vehicle.getName());
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
            getFuelForSupplier(vehicle);
            notifyAll();
        } else {
            while (vehicle.nitrogenFuelSupply > nitrogenFuel) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    System.out.println("Exception occured " + e);
                }
            }
            nitrogenFuel += vehicle.nitrogenFuelSupply;
            System.out.println(
                    "Station filled with " + vehicle.nitrogenFuelSupply + " of nitro by supplier " + vehicle.getName());
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
            getFuelForSupplier(vehicle);
            notifyAll();
        }
    }

    void getFuelForSupplier(SpaceVehicleSupplier vehicle) {
        if (vehicle.takesNitrogen && vehicle.takesQuantumFluid) {
            nitrogenFuel -= vehicle.nitorgenFuelTankCapacity; // take away fuel from station
            quantumFluid -= vehicle.quantumFluidTankCapacity;
            vehicle.nitrogenFuelTank = vehicle.nitorgenFuelTankCapacity; // Fuel the vehicle
            vehicle.quantumFluidTank = vehicle.quantumFluidTankCapacity;
            System.out.println("supplier " + vehicle.getName() + " refueled at station, has " + vehicle.nitrogenFuelTank
                    + " of nitro & " + vehicle.quantumFluidTank + " of quantum");
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
        } else if (vehicle.takesQuantumFluid) {
            quantumFluid -= vehicle.quantumFluidTankCapacity;
            vehicle.quantumFluidTank = vehicle.quantumFluidTankCapacity;
            System.out.println("supplier " + vehicle.getName() + " refueled at station, has " + vehicle.quantumFluidTank
                    + " of quantum");
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
        } else {
            nitrogenFuel -= vehicle.nitorgenFuelTankCapacity; // take away fuel from station
            vehicle.nitrogenFuelTank = vehicle.nitorgenFuelTankCapacity; // Fuel the vehicle
            System.out.println("supplier " + vehicle.getName() + " refueled at station, has " + vehicle.nitrogenFuelTank
                    + " of nitro");
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
        }
    }

    synchronized void getFuel(SpaceVehicle vehicle) {
        if (vehicle.takesNitrogen && vehicle.takesQuantumFluid) {
            while (vehicle.nitorgenFuelTankCapacity > nitrogenFuel || vehicle.quantumFluidTankCapacity > quantumFluid) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    System.out.println("Exception occured " + e);
                }
            }
            nitrogenFuel -= vehicle.nitorgenFuelTankCapacity; // take away fuel from station
            quantumFluid -= vehicle.quantumFluidTankCapacity;
            vehicle.nitrogenFuelTank = vehicle.nitorgenFuelTankCapacity; // Fuel the vehicle
            vehicle.quantumFluidTank = vehicle.quantumFluidTankCapacity;
            System.out.println("vehicle " + vehicle.getName() + " refueled at station, has " + vehicle.nitrogenFuelTank
                    + " of nitro & " + vehicle.quantumFluidTank + " of quantum");
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
            notifyAll();
        } else if (vehicle.takesQuantumFluid) {
            while (vehicle.quantumFluidTankCapacity > quantumFluid) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    System.out.println("Exception occured " + e);
                }
            }
            quantumFluid -= vehicle.quantumFluidTankCapacity;
            vehicle.quantumFluidTank = vehicle.quantumFluidTankCapacity;
            System.out.println("vehicle " + vehicle.getName() + "refueled at station, has " + vehicle.quantumFluidTank
                    + " of quantum");
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
            notifyAll();
        } else {
            while (vehicle.nitorgenFuelTankCapacity > nitrogenFuel) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    System.out.println("Exception occured " + e);
                }
            }
            nitrogenFuel -= vehicle.nitorgenFuelTankCapacity; // take away fuel from station
            vehicle.nitrogenFuelTank = vehicle.nitorgenFuelTankCapacity; // Fuel the vehicle
            System.out.println("vehicle " + vehicle.getName() + "refueled at station, has " + vehicle.nitrogenFuelTank
                    + " of nitro");
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
            notifyAll();
        }
    }

    public static void main(String[] args) {
        Random random = new Random();
        int min = 10;
        int max = 50;

        SpaceVehicle[] vehiclePacker = new SpaceVehicle[5];
        SpaceVehicleSupplier[] supplierPacker = new SpaceVehicleSupplier[2];

        SpaceStation station = new SpaceStation(4, 70, 60);
        for (int i = 0; i < 5; i++) {
            int nitorgenFuelTankCapacity = random.nextInt(max - min) + min;
            int quantumFluidTankCapacity = random.nextInt(max - min) + min;
            vehiclePacker[i] = new SpaceVehicle(Integer.toString(i), nitorgenFuelTankCapacity, quantumFluidTankCapacity,
                    station);
            vehiclePacker[i].start();
        }

        supplierPacker[0] = new SpaceVehicleSupplier(20, 25, 60, 65, station);
        supplierPacker[1] = new SpaceVehicleSupplier(0, 40, 55, 60, station);
        supplierPacker[0].start();
        supplierPacker[1].start();

        for (int i = 0; i < vehiclePacker.length; i++) {
            try {
                vehiclePacker[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (int i = 0; i < supplierPacker.length; i++) {
            try {
                supplierPacker[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

}
