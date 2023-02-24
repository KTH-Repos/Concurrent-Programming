public class Bear extends Thread {
    HoneyPot pot;

    public Bear(HoneyPot pot) {
        this.pot = pot;
    }

    @Override
    public void run() {
        pot.takeHoney(this);
        try {
            sleep(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
