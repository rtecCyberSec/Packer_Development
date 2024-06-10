use clroxide::clr::Clr;

fn assembly_load(bytes_to_load: Vec<u8>) -> Result<(), String> {
    let mut args: Vec<String> = env::args().collect();
    // remove first arg, as this is the path to the binary
    args.remove(0);
    
    let mut clr = Clr::new(bytes_to_load, args)?;

    let results = clr.run()?;

    println!("{}", results);

    Ok(())
}
