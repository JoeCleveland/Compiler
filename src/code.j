

fn rec (int x) {
    if x == 1 | x == 0 {
        ret x;
    }
    else {
        ret rec (x - 1) + rec (x - 2);
    }
}
fn main {
    ret rec(9);
}

