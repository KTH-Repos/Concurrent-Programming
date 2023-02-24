import java.util.ArrayList;
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
    int nitrogenFuelMax;
    int quantumFluidMax;
    int quantumFluid;
    Random random;

    public SpaceStation() {
        this.docks = 4;
        this.nitrogenFuelMax = 100;
        this.quantumFluidMax = 100;
        this.random = new Random();
    }

    synchronized void setFuel(SpaceVehicleSupplier vehicle) {
        int min = 1;
        int max = 3;
        if (vehicle.suppliesNitorgen && vehicle.suppliesQuantumFluid) {
            while ((vehicle.nitrogenFuelSupply - nitrogenFuel < 0) || (vehicle.quantumFluidSupply - quantumFluid < 0)
                    || docks == 0) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    System.out.println("Exception occured " + e);
                }
            }
            docks--;
            int sleepDuration = random.nextInt(max - min) + min;
            try {
                Thread.sleep(sleepDuration * 1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            nitrogenFuel += vehicle.nitrogenFuelSupply;
            quantumFluid += vehicle.quantumFluidSupply;
            System.out.println("Station filled with " + vehicle.nitrogenFuelSupply + " of nitro & "
                    + vehicle.quantumFluidSupply + " of quantum by supplier " + vehicle.getName());
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
            getFuelForSupplier(vehicle);
            System.out.println("Supplier " + vehicle.getName() + " will now travel away ...");
            System.out.println("  ");
            docks++;
            notifyAll();
        } else if (vehicle.suppliesQuantumFluid) {
            while ((vehicle.quantumFluidSupply - quantumFluid < 0) || docks == 0) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    System.out.println("Exception occured " + e);
                }
            }
            docks--;
            int sleepDuration = random.nextInt(max - min) + min;
            try {
                Thread.sleep(sleepDuration * 1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            quantumFluid += vehicle.quantumFluidSupply;
            System.out.println("Station filled with " + vehicle.quantumFluidSupply + " of quantum by supplier "
                    + vehicle.getName());
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
            getFuelForSupplier(vehicle);
            System.out.println("Supplier " + vehicle.getName() + " will now travel away ...");
            System.out.println("  ");
            docks++;
            notifyAll();
        } else {
            while ((vehicle.nitrogenFuelSupply - nitrogenFuel < 0) || docks == 0) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    System.out.println("Exception occured " + e);
                }
            }
            docks--;
            int sleepDuration = random.nextInt(max - min) + min;
            try {
                Thread.sleep(sleepDuration * 1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            nitrogenFuel += vehicle.nitrogenFuelSupply;
            System.out.println(
                    "Station filled with " + vehicle.nitrogenFuelSupply + " of nitro by supplier " + vehicle.getName());
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
            getFuelForSupplier(vehicle);
            System.out.println("Supplier " + vehicle.getName() + " will now travel away ...");
            System.out.println("  ");
            docks++;
            notifyAll();
        }
    }

    void getFuelForSupplier(SpaceVehicleSupplier vehicle) {
        if (vehicle.takesNitrogen && vehicle.takesQuantumFluid) {
            /*
             * while (vehicle.nitorgenFuelTankCapacity > nitrogenFuel ||
             * vehicle.quantumFluidTankCapacity > quantumFluid) {
             * try {
             * wait();
             * } catch (InterruptedException e) {
             * System.out.println("Exception occured " + e);
             * }
             * }
             */
            nitrogenFuel -= vehicle.nitorgenFuelTankCapacity; // take away fuel from station
            quantumFluid -= vehicle.quantumFluidTankCapacity;
            vehicle.nitrogenFuelTank = vehicle.nitorgenFuelTankCapacity; // Fuel the vehicle
            vehicle.quantumFluidTank = vehicle.quantumFluidTankCapacity;
            System.out.println("supplier " + vehicle.getName() + " refueled at station, has " + vehicle.nitrogenFuelTank
                    + " of nitro & " + vehicle.quantumFluidTank + " of quantum");
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
        } else if (vehicle.takesQuantumFluid) {
            /*
             * while (vehicle.quantumFluidTankCapacity > quantumFluid) {
             * try {
             * wait();
             * } catch (InterruptedException e) {
             * System.out.println("Exception occured " + e);
             * }
             * }
             */
            quantumFluid -= vehicle.quantumFluidTankCapacity;
            vehicle.quantumFluidTank = vehicle.quantumFluidTankCapacity;
            System.out.println("supplier " + vehicle.getName() + " refueled at station, has " + vehicle.quantumFluidTank
                    + " of quantum");
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
        } else {
            /*
             * while (vehicle.nitorgenFuelTankCapacity > nitrogenFuel) {
             * try {
             * wait();
             * } catch (InterruptedException e) {
             * System.out.println("Exception occured " + e);
             * }
             * }
             */
            nitrogenFuel -= vehicle.nitorgenFuelTankCapacity; // take away fuel from station
            vehicle.nitrogenFuelTank = vehicle.nitorgenFuelTankCapacity; // Fuel the vehicle
            System.out.println("supplier " + vehicle.getName() + " refueled at station, has " + vehicle.nitrogenFuelTank
                    + " of nitro");
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
        }
    }

    synchronized void getFuel(SpaceVehicle vehicle) {
        int min = 1;
        int max = 3;
        if (vehicle.takesNitrogen && vehicle.takesQuantumFluid) {
            while (vehicle.nitorgenFuelTankCapacity > nitrogenFuel || vehicle.quantumFluidTankCapacity > quantumFluid
                    || docks == 0) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    System.out.println("Exception occured " + e);
                }
            }
            docks--;
            int sleepDuration = random.nextInt(max - min) + min;
            System.out.println("vehicle sleeps for " + sleepDuration + " ms");
            try {
                Thread.sleep(sleepDuration * 1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            nitrogenFuel -= vehicle.nitorgenFuelTankCapacity; // take away fuel from station
            quantumFluid -= vehicle.quantumFluidTankCapacity;
            vehicle.nitrogenFuelTank = vehicle.nitorgenFuelTankCapacity; // Fuel the vehicle
            vehicle.quantumFluidTank = vehicle.quantumFluidTankCapacity;
            System.out.println("vehicle " + vehicle.getName() + " refueled at station, has " + vehicle.nitrogenFuelTank
                    + " of nitro & " + vehicle.quantumFluidTank + " of quantum");
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
            System.out.println("Vehicle " + vehicle.getName() + " will now travel away ...");
            System.out.println("  ");
            docks++;
            notifyAll();
        } else if (vehicle.takesQuantumFluid) {
            while (vehicle.quantumFluidTankCapacity > quantumFluid || docks == 0) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    System.out.println("Exception occured " + e);
                }
            }
            docks--;
            int sleepDuration = random.nextInt(max - min) + min;
            try {
                Thread.sleep(sleepDuration * 1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            quantumFluid -= vehicle.quantumFluidTankCapacity;
            vehicle.quantumFluidTank = vehicle.quantumFluidTankCapacity;
            System.out.println("vehicle " + vehicle.getName() + "refueled at station, has " + vehicle.quantumFluidTank
                    + " of quantum");
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
            System.out.println("Vehicle " + vehicle.getName() + " will now travel away ...");
            System.out.println("  ");
            docks++;
            notifyAll();
        } else {
            while (vehicle.nitorgenFuelTankCapacity > nitrogenFuel || docks == 0) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    System.out.println("Exception occured " + e);
                }
            }
            docks--;
            int sleepDuration = random.nextInt(max - min) + min;
            try {
                Thread.sleep(sleepDuration * 1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            nitrogenFuel -= vehicle.nitorgenFuelTankCapacity; // take away fuel from station
            vehicle.nitrogenFuelTank = vehicle.nitorgenFuelTankCapacity; // Fuel the vehicle
            System.out.println("vehicle " + vehicle.getName() + "refueled at station, has " + vehicle.nitrogenFuelTank
                    + " of nitro");
            System.out.println("Station has now " + nitrogenFuel + " of nitro & " + quantumFluid + " of quantum");
            System.out.println("Vehicle " + vehicle.getName() + " will now travel away ...");
            System.out.println("  ");
            docks++;
            notifyAll();
        }
    }

    public static void main(String[] args) {
        Random random = new Random();
        int min = 10;
        int max = 30;

        ArrayList<SpaceVehicle> vehiclePacker = new ArrayList<>();
        ArrayList<SpaceVehicleSupplier> supplierPacker = new ArrayList<>();

        SpaceStation station = new SpaceStation();

        for (int i = 0; i < 9; i++) {
            int nitorgenFuelTankCapacity = random.nextInt(max - min) + min;
            int quantumFluidTankCapacity = random.nextInt(max - min) + min;
            if (i < 3) {
                vehiclePacker
                        .add(new SpaceVehicle(Integer.toString(i), nitorgenFuelTankCapacity, quantumFluidTankCapacity,
                                station));
            } else if (i >= 3 && i < 6) {
                vehiclePacker.add(new SpaceVehicle(Integer.toString(i), nitorgenFuelTankCapacity, 0, station));
            } else {
                vehiclePacker.add(new SpaceVehicle(Integer.toString(i), 0, quantumFluidTankCapacity, station));
            }
        }

        supplierPacker.add(new SpaceVehicleSupplier(Integer.toString(0), 20, 25, 60, 65, station));
        supplierPacker.add(new SpaceVehicleSupplier(Integer.toString(1), 0, 15, 55, 60, station));
        supplierPacker.add(new SpaceVehicleSupplier(Integer.toString(2), 25, 0, 0, 60, station));
        supplierPacker.add(new SpaceVehicleSupplier(Integer.toString(3), 0, 10, 75, 0, station));

        for (int i = 0; i < vehiclePacker.size(); i++) {
            vehiclePacker.get(i).start();
        }

        for (int i = 0; i < supplierPacker.size(); i++) {
            supplierPacker.get(i).start();
        }

        for (int i = 0; i < vehiclePacker.size(); i++) {
            try {
                vehiclePacker.get(i).join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            ;
        }

        for (int i = 0; i < supplierPacker.size(); i++) {
            try {
                supplierPacker.get(i).join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.println("nitro left is " + station.nitrogenFuel);
        System.out.println("quantum left is " + station.quantumFluid);
    }

}
