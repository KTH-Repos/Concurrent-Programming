public class Bees extends Thread {

    HoneyPot pot;

    public Bees(String name, HoneyPot pot) {
        super(name);
        this.pot = pot;
    }

    @Override
    public void run() {
        pot.putHoney(this);
    }
}
