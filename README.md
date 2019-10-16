# re-bundler


[![CircleCI](https://circleci.com/gh/yourgithubhandle/re-bundler/tree/master.svg?style=svg)](https://circleci.com/gh/yourgithubhandle/re-bundler/tree/master)


**Contains the following libraries and executables:**

```
re-bundler@0.0.0
│
├─test/
│   name:    TestReBundler.exe
│   main:    TestReBundler
│   require: re-bundler.lib
│
├─library/
│   library name: re-bundler.lib
│   namespace:    ReBundler
│   require:
│
└─executable/
    name:    ReBundlerApp.exe
    main:    ReBundlerApp
    require: re-bundler.lib
```

## Developing:

```
npm install -g esy
git clone <this-repo>
esy install
esy build
```

## Running Binary:

After building the project, you can run the main binary that is produced.

```
esy x ReBundlerApp.exe 
```

## Running Tests:

```
# Runs the "test" command in `package.json`.
esy test
```
