const { EventEmitter } = require("events");

class DB extends EventEmitter {
  constructor() {
    super();
    this.table = [
      { id: 1, name: "name1", bday: "12.10.2025" },
      { id: 2, name: "name2", bday: "12.10.2025" },
      { id: 3, name: "name3", bday: "12.10.2025" },
    ];
    this.commitCount = 0;
  }

  async select() {
    return this.table;
  }

  async insert(item) {
    if (!item.name || !item.bday || isNaN(Number(item.id))) {
      throw new Error("Insert: no bday or name or id or something");
    }
    if(this.table.find(i=>i.id === item.id))
    {
      throw new Error(`Insert: duplicate id ${item.id} found, choose another one `);
    }
    let insertItem = { id: item.id, name: item.name, bday: item.bday };
    // this.newId++;
    this.table.push(insertItem);
    return insertItem;
  }

  async update(newItem) {
    let toUpdate = this.table.findIndex((item) => item.id === newItem.id);
    if (toUpdate === -1) {
      throw new Error("Update: no item to update found");
    }
    if (newItem.bday !== null && newItem.bday !== undefined) {
      this.table[toUpdate].bday = newItem.bday;
    }
    if (newItem.name !== null && newItem.name !== undefined) {
      this.table[toUpdate].name = newItem.name;
    }
    return this.table[toUpdate];
  }

  async delete(id) {
    let toDelete = this.table.findIndex((item) => item.id === id);
    if (toDelete === -1) {
      throw new Error("Delete: no item to delete found");
    }
    let deleted = this.table.splice(toDelete, 1)[0];
    return deleted;
  }
  async commit()
  {
    this.commitCount++;
    return this.commitCount;
  }

  
  
}

module.exports = DB;
