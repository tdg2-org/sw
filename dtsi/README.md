vivado/vitis 2025.1

#### device tree appends for IP not in BD
* do not name the file 'pl.dtsi', there is an auto-generated file of this name already, will be ignored.

```
&i2c_pl {
  compatible = "xlnx,axi-iic-2.1", "xlnx,xps-iic-2.00.a";
  reg = <0x0 0xa0001000 0x0 0x200>;
  status = "okay";
  xlnx,ten-bit-adr = <0>;
  xlnx,gpo-width = <1>;
  clocks = <&zynqmp_clk 71>;
  clock-names = "s_axi_aclk";
};
```

* this is an append to an already existing node in the auto-generated device tree files from XSA
* * axi address out of BD must be declared in BD, tested with my passthrough IP
* * i2c_pl is name in address editor and axi interface


#### FAIL - excluded addresses in BD
I attempted to add a node fully custom while having the address excluded in the vivado build. This fails on HW
for that specific node/IP, which makes sense - how would the address be routed through the smartconnect?
'rtl-ip-no-passthru.dtsi' shows how to add a new node, not append. This may work if the address is not excluded
in vivado, but it requires the full cpu address-map copied from system-top.dts (vitis platform/sources/hw/sdt), 
then add the custom value...
* the append method is much cleaner/simpler
* not saving SW test for this