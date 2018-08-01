# compiler
This is the implementation of a compiler for a language whose design is a work in progress.
The syntax will look something like this:
```
type Node<K> {
  K obj;
  Node next;
}

met:Node<K> insert(K o) {
  Node p = next;
  while p.next != null {
    p = p.next;
  }
  p.next = Node<K>(o, null);
}

fn main {
  Node<int> list(0, null);
  list.insert(1);
  list.insert(2);
}
```
