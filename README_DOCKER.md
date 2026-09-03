Docker
======

### Build the image

```bash
docker build -t [IMAGE_ID] . 
``` 
[<a href="#fn1" id="ref1">1</a>]

### Run the image

You can run the tools either interactively or directly via a one-shot command.

#### Option A: Interactively

``` bash
docker run -it --rm [IMAGE_ID] bash
```

Inside the container:

- **cruncher** (always available):

    ```bash
    echo "rosa" | MORPHLIB=stemlib bin/cruncher -L
    ```
    [<a href="#fn2" id="ref2">2</a>]

- **morpheus** XML output [<a href="#fn3" id="ref3">3</a>]

    ```bash
    echo "rosa" | MORPHLIB=stemlib bin/morpheus -L
    ```

#### Option B: One-shot command

You can also execute a run directly from your host without entering an 
interactive shell:

```bash
docker run --rm [IMAGE_ID] /bin/sh -c 'echo "rosa" | MORPHLIB=stemlib bin/cruncher -L'
```

### Footnotes

<ol>
  <li id="fn1">
    IMAGE_ID stands for any name you want to give 
    <a href="#ref1">↩</a>
  </li>
  <li id="fn2">
    "-L" selects Latin; drop it for Greek (the default). See the main
    README for the rest of the flags 
    <a href="#ref2">↩</a>
  </li>
  <li id="fn3">
    Only if this build includes it; check first
    with "ls bin/morpheus" inside the container, or "docker run --rm [IMAGE_ID] ls bin/" from outside 
    <a href="#ref3">↩</a>
  </li>
</ol>