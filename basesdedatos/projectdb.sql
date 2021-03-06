-- MySQL Script generated by MySQL Workbench
-- 07/23/14 12:55:14
-- Model: New Model    Version: 1.0
SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema projectdb
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `projectdb` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci ;
USE `projectdb` ;

-- -----------------------------------------------------
-- Table `projectdb`.`usuario`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `projectdb`.`usuario` (
  `idusuario` INT NOT NULL AUTO_INCREMENT,
  `nombre` VARCHAR(45) NULL,
  `apellido1` VARCHAR(45) NULL,
  `apellido2` VARCHAR(45) NULL,
  `user` VARCHAR(45) NULL,
  `password` VARCHAR(45) NULL,
  PRIMARY KEY (`idusuario`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `projectdb`.`raspberry`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `projectdb`.`raspberry` (
  `telefono` INT NOT NULL,
  `nombre` VARCHAR(45) NULL,
  `descripcion` VARCHAR(45) NULL,
  `usuario_idusuario` INT NOT NULL,
  PRIMARY KEY (`telefono`),
  INDEX `fk_raspberry_usuario_idx` (`usuario_idusuario` ASC),
  CONSTRAINT `fk_raspberry_usuario`
    FOREIGN KEY (`usuario_idusuario`)
    REFERENCES `projectdb`.`usuario` (`idusuario`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `projectdb`.`coche`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `projectdb`.`coche` (
  `id_vehiculo` INT NOT NULL AUTO_INCREMENT,
  `combustible` VARCHAR(45) NULL,
  `tipo` VARCHAR(45) NULL,
  `usuario_idusuario` INT NOT NULL,
  `raspberry_telefono` INT NOT NULL,
  PRIMARY KEY (`id_vehiculo`, `usuario_idusuario`),
  INDEX `fk_coche_usuario1_idx` (`usuario_idusuario` ASC),
  INDEX `fk_coche_raspberry1_idx` (`raspberry_telefono` ASC),
  CONSTRAINT `fk_coche_usuario1`
    FOREIGN KEY (`usuario_idusuario`)
    REFERENCES `projectdb`.`usuario` (`idusuario`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_coche_raspberry1`
    FOREIGN KEY (`raspberry_telefono`)
    REFERENCES `projectdb`.`raspberry` (`telefono`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `projectdb`.`gps`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `projectdb`.`gps` (
  `id_gps` INT NOT NULL AUTO_INCREMENT,
  `id_viaje` VARCHAR(45) NULL,
  `fecha_rpi` VARCHAR(45) NULL,
  `fecha_servidor` DATETIME NULL,
  `fecha_gps` VARCHAR(45) NULL,
  `UTCTime` VARCHAR(45) NULL,
  `latitud` VARCHAR(45) NULL,
  `N_S` VARCHAR(45) NULL,
  `longitud` VARCHAR(45) NULL,
  `E_W` VARCHAR(45) NULL,
  `velocidad` VARCHAR(45) NULL,
  `modo` VARCHAR(45) NULL,
  `estadoGPS` VARCHAR(45) NULL,
  `rumbo` VARCHAR(45) NULL,
  `distancia` VARCHAR(45) NULL,
  `km_recorridos` VARCHAR(45) NULL,
  `coche_id_vehiculo` INT NOT NULL,
  `coche_usuario_idusuario` INT NOT NULL,
  PRIMARY KEY (`id_gps`, `coche_id_vehiculo`, `coche_usuario_idusuario`),
  INDEX `fk_gps_coche1_idx` (`coche_id_vehiculo` ASC, `coche_usuario_idusuario` ASC),
  CONSTRAINT `fk_gps_coche1`
    FOREIGN KEY (`coche_id_vehiculo` , `coche_usuario_idusuario`)
    REFERENCES `projectdb`.`coche` (`id_vehiculo` , `usuario_idusuario`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `projectdb`.`obd`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `projectdb`.`obd` (
  `id_obd` INT NOT NULL AUTO_INCREMENT,
  `id_viaje` VARCHAR(45) NULL,
  `fecha_rpi` VARCHAR(45) NULL,
  `velocidad` VARCHAR(45) NULL,
  `velocidad_media` VARCHAR(45) NULL,
  `rpm` VARCHAR(45) NULL,
  `temperatura_motor` VARCHAR(45) NULL,
  `motor` VARCHAR(45) NULL,
  `MAF` VARCHAR(45) NULL,
  `MIL` VARCHAR(45) NULL,
  `inyeccion_directa` VARCHAR(45) NULL,
  `inyeccion` VARCHAR(45) NULL,
  `consumo` VARCHAR(45) NULL,
  `eficiencia_diesel` VARCHAR(45) NULL,
  `eficiencia_gasolina` VARCHAR(45) NULL,
  `DTC` VARCHAR(45) NULL,
  `fecha_servidor` DATETIME NULL,
  `coche_id_vehiculo` INT NOT NULL,
  `coche_usuario_idusuario` INT NOT NULL,
  `throttle` VARCHAR(45) NULL,
  `intake_air` VARCHAR(45) NULL,
  `lambda_o2s1_v` VARCHAR(45) NULL,
  `lambda_o2s8_v` VARCHAR(45) NULL,
  `egr` VARCHAR(45) NULL,
  `lambda_o2s3_c` VARCHAR(45) NULL,
  `run_time` VARCHAR(45) NULL,
  `fuel_level` VARCHAR(45) NULL,
  PRIMARY KEY (`id_obd`, `coche_id_vehiculo`, `coche_usuario_idusuario`),
  INDEX `fk_obd_coche1_idx` (`coche_id_vehiculo` ASC, `coche_usuario_idusuario` ASC),
  CONSTRAINT `fk_obd_coche1`
    FOREIGN KEY (`coche_id_vehiculo` , `coche_usuario_idusuario`)
    REFERENCES `projectdb`.`coche` (`id_vehiculo` , `usuario_idusuario`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
