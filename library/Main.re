open Tablecloth;

let readFiles = (~dirName) => {
  print_endline(
    "Searching in " ++ Sys.getcwd() ++ Filename.dir_sep ++ dirName,
  );
  let newDir = Sys.getcwd() ++ "/project";

  Sys.chdir(newDir);
  newDir
  |> Lwt_unix.files_of_directory
  |> Lwt_stream.iter(s => {
       s |> Sys.is_directory
         ? print_endline("dir " ++ s) : print_endline("file " ++ s)
     })
  |> Lwt_main.run;
};

let rec really_read = (fd, buffer, start, length) => {
  let bytes = Lwt_bytes.create(buffer);
  Lwt_unix.read(fd, bytes |> Lwt_bytes.to_bytes, 0, buffer)
  |> Lwt.map(rest =>
       switch (rest) {
       | 0 => ""
       | r => really_read(fd, buffer, start + r, length - r)
       }
     )
  |> ignore;
  bytes |> Lwt_bytes.to_string;
};

let getEntryPoint = entry => {
  let file = Sys.getcwd() ++ Filename.dir_sep ++ entry;

  //iter_n

  file
  |> Lwt_io.lines_of_file
  |> Lwt_stream.map(Soup.parse)
  |> Lwt_stream.map(s =>
       Soup.(
         with_stop(stop => {
           s
           |> select("script")
           |> elements
           |> iter(element =>
                if (has_attribute("src", element)) {
                  print_endline(
                    element
                    |> attribute("src")
                    |> Option.withDefault(~default=""),
                  );
                  stop.throw(Some(element |> attribute("src")));
                }
              );
           None;
         })
       )
     );
};
let readConfig = () =>
  Yojson.Basic.(
    {
      print_endline(Sys.getcwd());
      let file =
        Sys.getcwd()
        ++ Filename.dir_sep
        ++ "project-to-bundle/rebundler.config.json";
      let json = file |> from_file;
      // print_endline(json |> pretty_to_string);
      json |> Util.member("entryPoint") |> Util.to_string_option;
    }
  );
let time = (f, x) => {
  let t = Sys.time() *. 1000.0;
  let fx = f(x);
  Printf.printf("Execution time: %fms\n", Sys.time() *. 1000.0 -. t);
  fx;
};

let run = () => {
  switch (readConfig()) {
  | Some(entryPointPath) =>
    time(getEntryPoint, entryPointPath)
    |> Lwt_stream.iter(ignore)
    |> Lwt_main.run
  //todo: throw nice error, wrong config
  | _ => ()
  };
};